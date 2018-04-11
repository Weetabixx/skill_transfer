#ifndef PLUGINS_TILTGRABPLUGIN_H
#define PLUGINS_TILTGRABPLUGIN_H


#include <gazebo/gazebo.hh>
#include <gazebo/physics/Joint.hh>
#include <gazebo/sensors/sensors.hh>

namespace gazebo {
    class TiltGrabPlugin : public ModelPlugin {
    public:
        TiltGrabPlugin();
        void Load(physics::ModelPtr _parent, sdf::ElementPtr _sdf) override;
        void OnUpdate(const common::UpdateInfo & _info);
        void Reset() override;
        void CreateFirstJoint();
        void CreateSecondJoints();
        void BreakJoint();

    private:
        physics::PhysicsEnginePtr physics;
        physics::ModelPtr model;
        physics::ModelPtr book_model;
        physics::JointPtr joint1;
        physics::JointPtr joint2;
        physics::JointPtr joint3;
        physics::LinkPtr childLink1;
        physics::LinkPtr childLink2;
        physics::LinkPtr childLink3;
        physics::LinkPtr parentLink;
        event::ConnectionPtr updateConnection;
        int grabPhase;
        sensors::ContactSensorPtr parentSensor;
        physics::ContactManager *cMgr;
        bool curcontact;
        bool left_finger_touching;
        bool right_fingers_touching;
        double goalZ;
    };
}


#endif //PLUGINS_TILTGRABPLUGIN_H
