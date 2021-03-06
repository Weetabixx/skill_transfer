#include <ros/ros.h>
#include <actionlib/server/simple_action_server.h>
#include <skill_transfer/MoveArmAction.h>
#include <geometry_msgs/Twist.h>
#include <sensor_msgs/JointState.h>
#include <visualization_msgs/Marker.h>
#include <giskard_core/giskard_core.hpp>
#include "skill_transfer/conversions.h"
#include "skill_transfer/giskard_adapter.h"
#include <vector>
#include <string>
#include <algorithm>
#include "skill_transfer/watchdog.hpp"

class ConstraintController
{
public:
  ConstraintController(std::string name) : as_(nh_, name, false),
                                           action_name_(name),
                                           giskard_adapter_(100)
  {
    joint_names_ = {
        "torso_lift_joint",
        "l_shoulder_pan_joint",
        "l_shoulder_lift_joint",
        "l_upper_arm_roll_joint",
        "l_elbow_flex_joint",
        "l_forearm_roll_joint",
        "l_wrist_flex_joint",
        "l_wrist_roll_joint",
        "r_shoulder_pan_joint",
        "r_shoulder_lift_joint",
        "r_upper_arm_roll_joint",
        "r_elbow_flex_joint",
        "r_forearm_roll_joint",
        "r_wrist_flex_joint",
        "r_wrist_roll_joint"};

    //register the goal and feeback callbacks
    as_.registerGoalCallback(boost::bind(&ConstraintController::onGoal, this));
    as_.registerPreemptCallback(boost::bind(&ConstraintController::onPreempt, this));

    //subscribe to the data topic of interest
    sub_ = nh_.subscribe("/joint_states", 1, &ConstraintController::onJointStatesMsg, this,
                         ros::TransportHints().tcpNoDelay());

    // Topic for real PR2 commands (joint velocities)
    pub_ = nh_.advertise<sensor_msgs::JointState>("/whole_body_controller/velocity_controller/command", 1);
    // Topic for simulation and executive node, since they only
    // care about the end effector velocity and not about joint velocities
    pub_gripper_ = nh_.advertise<geometry_msgs::Twist>("/set_l_ee_twist", 1);
    pub_gripper_measured_ = nh_.advertise<geometry_msgs::Twist>("/l_ee_twist", 1);
    // Desired motion state visualization for RViz
    pub_viz_ = nh_.advertise<visualization_msgs::Marker>("/giskard/visualization_marker", 1);

    watchdog_.setPeriod(ros::Duration(0.1));

    as_.start();
  }

  ~ConstraintController()
  {
  }

  void onGoal()
  {
    // Accept goal and get new constraints
    const auto goal = as_.acceptNewGoal();
    constraints_ = goal->constraints;

    ROS_INFO("%s: Received a new goal", action_name_.c_str());

    giskard_adapter_.createController(constraints_);
    watchdog_.kick(ros::Time::now());
  }

  void onPreempt()
  {
    ROS_INFO("%s: Preempted", action_name_.c_str());
    // set the action state to preempted
    as_.setPreempted();
  }

  void onJointStatesMsg(const sensor_msgs::JointStateConstPtr &msg)
  {
    if (watchdog_.barking(msg->header.stamp))
    {
      // ROS_INFO("BARKING");
      return;
    }

    // Link state map
    auto joint_positions_map = toMap<std::string, double>(msg->name, msg->position);
    auto joint_velocities_map = toMap<std::string, double>(msg->name, msg->velocity);

    auto joint_count = joint_names_.size();

    // When action is not active send zero twist,
    // otherwise do all the calculations
    if (as_.isActive())
    {
      // Prepare controller inputs

      Eigen::VectorXd inputs(joint_count);

      for (int i = 0; i < joint_count; ++i)
      {
        inputs(i) = joint_positions_map.find(joint_names_[i])->second;
      }

      Eigen::VectorXd velocities(joint_count);

      for (int i = 0; i < joint_count; ++i)
      {
        velocities(i) = joint_velocities_map.find(joint_names_[i])->second;
      }

      // Start the controller if it's a new one
      if (!giskard_adapter_.controller_started_)
      {
        giskard_adapter_.startController(inputs);
      }

      // Get new calculations from the controller
      giskard_adapter_.updateController(inputs);

      const auto ee_twist_desired = giskard_adapter_.getDesiredFrameTwistMsg(inputs, "left_ee");
      const auto ee_twist_measured = giskard_adapter_.getMeasuredFrameTwistMsg(inputs, velocities, "left_ee");
      const auto cmd = giskard_adapter_.getDesiredJointVelocityMsg();

      // ROS_INFO_STREAM("ee_twist_desired" << ee_twist_desired);

      pub_.publish(cmd);
      pub_gripper_.publish(ee_twist_desired);
      pub_gripper_measured_.publish(ee_twist_measured);

      feedback_.distance = giskard_adapter_.getDistance();
      as_.publishFeedback(feedback_);

      // Visualization
      const auto viz_msgs = giskard_adapter_.getVisualizationMsgs();

      for (const auto &m : viz_msgs)
      {
        pub_viz_.publish(m);
      }
    }
    else
    {
      Eigen::VectorXd velocities(joint_count);

      for (int i = 0; i < joint_count; ++i)
      {
        velocities(i) = 0.0;
      }

      auto cmd = eigenVectorToMsgJointState(velocities);

      pub_.publish(cmd);
    }

    watchdog_.kick(ros::Time::now());
    // ROS_INFO_STREAM("Twist: " << cmd.twist);
  }

protected:
  ros::NodeHandle nh_;
  actionlib::SimpleActionServer<skill_transfer::MoveArmAction> as_;
  std::string action_name_;
  ros::Subscriber sub_;
  ros::Publisher pub_;
  ros::Publisher pub_gripper_;
  ros::Publisher pub_gripper_measured_;
  ros::Publisher pub_viz_;
  std::string constraints_;
  skill_transfer::MoveArmFeedback feedback_;
  GiskardAdapter giskard_adapter_;
  std::vector<std::string> joint_names_;
  giskard_ros::Watchdog<ros::Time, ros::Duration> watchdog_;
};

int main(int argc, char **argv)
{
  ros::init(argc, argv, "constraint_controller");

  ConstraintController controller("move_arm");
  ros::spin();

  return 0;
}
