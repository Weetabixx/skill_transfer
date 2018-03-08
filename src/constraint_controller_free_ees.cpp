#include <ros/ros.h>
#include <actionlib/server/simple_action_server.h>
#include <skill_transfer/MoveArmAction.h>
#include <geometry_msgs/Twist.h>
#include <gazebo_msgs/LinkStates.h>
#include <visualization_msgs/Marker.h>
#include <giskard_core/giskard_core.hpp>
#include "skill_transfer/conversions.h"
#include "skill_transfer/giskard_adapter.h"
#include <vector>
#include <string>
#include <algorithm>

class ConstraintController
{
public:
  ConstraintController(std::string name) : as_(nh_, name, false),
                                           action_name_(name),
                                           giskard_adapter_(100)
  {
    //register the goal and feeback callbacks
    as_.registerGoalCallback(boost::bind(&ConstraintController::onGoal, this));
    as_.registerPreemptCallback(boost::bind(&ConstraintController::onPreempt, this));

    //subscribe to the data topic of interest
    sub_ = nh_.subscribe("/gazebo/link_states", 1, &ConstraintController::onLinkStatesMsg, this);
    // Topic for simulation and executive node, since they only
    // care about the end effector velocity and not about joint velocities
    pub_l_ee_ = nh_.advertise<geometry_msgs::Twist>("/l_ee_twist", 1);
    pub_set_l_ee_ = nh_.advertise<geometry_msgs::Twist>("/set_l_ee_twist", 1);
    pub_r_ee_ = nh_.advertise<geometry_msgs::Twist>("/r_ee_twist", 1);
    pub_set_r_ee_ = nh_.advertise<geometry_msgs::Twist>("/set_r_ee_twist", 1);
    pub_r_ee_2_ = nh_.advertise<geometry_msgs::Twist>("/r_ee_2_twist", 1);
    pub_set_r_ee_2_ = nh_.advertise<geometry_msgs::Twist>("/set_r_ee_2_twist", 1);
    // Desired motion state visualization for RViz
    pub_viz_ = nh_.advertise<visualization_msgs::Marker>("/giskard/visualization_marker", 10);

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
  }

  void onPreempt()
  {
    ROS_INFO("%s: Preempted", action_name_.c_str());
    // set the action state to preempted
    as_.setPreempted();
  }

  void onLinkStatesMsg(const gazebo_msgs::LinkStatesConstPtr &msg)
  {
    // Link state map
    auto link_pose_map = toMap<std::string, geometry_msgs::Pose>(msg->name, msg->pose);
    auto link_twist_map = toMap<std::string, geometry_msgs::Twist>(msg->name, msg->twist);
    
    const auto left_ee_pose = link_pose_map.find("left_ee::link")->second;
    const auto left_ee_twist = link_twist_map.find("left_ee::link")->second;
    const auto right_ee_pose = link_pose_map.find("right_ee::link")->second; 
    const auto right_ee_twist = link_twist_map.find("right_ee::link")->second; 
    const auto right_ee_2_pose = link_pose_map.find("right_ee_2::link")->second; 
    const auto right_ee_2_twist = link_twist_map.find("right_ee_2::link")->second; 

    // When action is not active send zero twist,
    // otherwise do all the calculations
    if (as_.isActive())
    {
      // Prepare controller inputs
      Eigen::VectorXd inputs(12);
      inputs.segment(0, 6) = msgPoseToEigenVector(left_ee_pose);
      inputs.segment(6, 6) = msgPoseToEigenVector(right_ee_pose);
      inputs.segment(6, 6) = msgPoseToEigenVector(right_ee_2_pose);
      
      // Start the controller if it's a new one
      if (!giskard_adapter_.controller_started_)
      {
        giskard_adapter_.startController(inputs);
      }

      // Get new calculations from the controller
      giskard_adapter_.updateController(inputs);
      
      const auto l_ee_twist_desired_msg = giskard_adapter_.getDesiredFrameTwistMsg(inputs, "left_ee");
      const auto r_ee_twist_desired_msg = giskard_adapter_.getDesiredFrameTwistMsg(inputs, "right_ee");
      const auto r_ee_2_twist_desired_msg = giskard_adapter_.getDesiredFrameTwistMsg(inputs, "right_ee_2");
    
      pub_set_l_ee_.publish(l_ee_twist_desired_msg);
      pub_l_ee_.publish(left_ee_twist);
      pub_set_r_ee_.publish(r_ee_twist_desired_msg);
      pub_r_ee_.publish(right_ee_twist);
      pub_set_r_ee_2_.publish(r_ee_twist_desired_msg);
      pub_r_ee_2_.publish(right_ee_twist);

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
      const geometry_msgs::Twist cmd;
      pub_set_l_ee_.publish(cmd);
      pub_set_r_ee_.publish(cmd);
      pub_set_r_ee_2_.publish(cmd);
    } 

    // ROS_INFO_STREAM("Twist: " << cmd.twist);
  }

protected:
  ros::NodeHandle nh_;
  actionlib::SimpleActionServer<skill_transfer::MoveArmAction> as_;
  std::string action_name_;
  ros::Subscriber sub_;
  ros::Publisher pub_l_ee_;
  ros::Publisher pub_set_l_ee_;
  ros::Publisher pub_r_ee_;
  ros::Publisher pub_set_r_ee_;
  ros::Publisher pub_r_ee_2_;
  ros::Publisher pub_set_r_ee_2_;
  ros::Publisher pub_viz_;
  std::string constraints_;
  skill_transfer::MoveArmFeedback feedback_;
  GiskardAdapter giskard_adapter_;
};

int main(int argc, char **argv)
{
  ros::init(argc, argv, "constraint_controller");

  ConstraintController controller("move_arm");
  ros::spin();

  return 0;
}
