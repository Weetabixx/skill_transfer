#ifndef PLUGINS_TILTGRABPLUGIN_H
#define PLUGINS_TILTGRABPLUGIN_H


#include <gazebo/gazebo.hh>
#include <gazebo/physics/Joint.hh>

namespace gazebo {
    class TiltGrabPlugin : public ModelPlugin {
    public:
        TiltGrabPlugin();
        void Load(physics::ModelPtr _parent, sdf::ElementPtr _sdf) override;
        void OnUpdate(const common::UpdateInfo & _info);
        void Reset() override;
        void CreateJoint();
        void BreakJoint();

    private:
        physics::PhysicsEnginePtr physics;
        physics::ModelPtr model;
        physics::JointPtr joint;
        physics::LinkPtr childLink;
        physics::LinkPtr parentLink;
        event::ConnectionPtr updateConnection;
        double forceThreshold;
    };
}


#endif //PLUGINS_TILTGRABPLUGIN_H
