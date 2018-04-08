#include "TiltGrabPlugin.hh"

#include <gazebo/physics/physics.hh>
#include <gazebo/sensors/sensors.hh>
#include <string>
#include <ros/ros.h>

using namespace gazebo;

// Register this plugin with the simulator
GZ_REGISTER_MODEL_PLUGIN(TiltGrabPlugin);


TiltGrabPlugin::TiltGrabPlugin(): ModelPlugin(), joint1(nullptr) {

}

void TiltGrabPlugin::Load(physics::ModelPtr _parent, sdf::ElementPtr _sdf) {
    ROS_INFO("Hello World!");
    this->model = _parent;
    const auto world = this->model->GetWorld();
    this->physics = world->GetPhysicsEngine();
    this->cMgr = this->physics->GetContactManager();
    if (!this->cMgr)
    {
        std::cout << "oops \n";
        gzerr << "nullptr. \n";
        return;
    }
    sensors::SensorManager *mgr = gazebo::sensors::SensorManager::Instance();
    
    const std::string childLinkName1 = _sdf->GetElement("childLinkName1")->Get<std::string>();
    const std::string childLinkName2 = _sdf->GetElement("childLinkName2")->Get<std::string>();
    const std::string childLinkName3 = _sdf->GetElement("childLinkName3")->Get<std::string>();
    const std::string parentLinkName = _sdf->GetElement("parentLinkName")->Get<std::string>();
    const std::string SensorName = _sdf->GetElement("sensorName")->Get<std::string>();
    //int y = model->GetSensorCount();
    //std::cout << y << "\n";
    //std::cout << SensorName << "\n";
    sensors::Sensor_V all = mgr->GetSensors();
    //std::cout << "used mgr \n";
    //std::string name;
    std::cout << all.size() << "\n";
    //for(int i = 0; i < all.size(); i++)
    //{
    //    name = all[i]->Name();
    //    std::cout << name << "\n";
    //}
    this->grabPhase = 0;

    this->parentLink = this->model->GetLink(parentLinkName);
    this->childLink1 = boost::dynamic_pointer_cast<physics::Link>(world->GetEntity(childLinkName1));
    this->childLink2 = boost::dynamic_pointer_cast<physics::Link>(world->GetEntity(childLinkName2));
    this->childLink3 = boost::dynamic_pointer_cast<physics::Link>(world->GetEntity(childLinkName3));
    //sensors::SensorPtr SensorPointer = mgr->GetSensor(SensorName);
    //if (!SensorPointer)
    //    {
    //        std::cout << "oops \n";
    //        gzerr << "nullptr. \n";
    //        return;
    //    }
    //this->parentSensor = std::dynamic_pointer_cast<sensors::ContactSensor>(SensorPointer);


    //this->updateConnection = this->parentSensor->ConnectUpdated(std::bind(&TiltGrabPlugin::OnUpdate, this));
    //this->parentSensor->SetActive(true);
    this->curcontact = true;
    this->updateConnection = event::Events::ConnectWorldUpdateBegin(
            boost::bind(&TiltGrabPlugin::OnUpdate, this, _1));

}

void TiltGrabPlugin::OnUpdate(const common::UpdateInfo &_info) {
    if (_info.simTime < 1.0) {
      // Let the stage settle down and position objects
      return;
    }
    std::vector<physics::Contact*> contacts;
    //physics::Contact contacts;
    //msgs::Contacts contacts;
    contacts = this->cMgr->GetContacts();
    int number = this->cMgr->GetContactCount();
    for (unsigned int i = 0; i < number; ++i)
    {
        physics::Collision *col1 = contacts[i]->collision1;
        physics::Collision *col2 = contacts[i]->collision2;
        std::string name1 = col1->GetName();
        std::string name2 = col2->GetName();
    	std::cout << "Collision between[" << name1 << "] and [" << name2 << "]\n";
        this->curcontact = true;
    }
    if (number == 0 and curcontact)
    {
        std::cout << "no Collisions \n";
        //std::cout << contacts << "\n";
        this->curcontact = false;
    }

    bool left_finger_touching = false;
    bool right_fingers_touching = false;
    if (this->grabPhase == 0){
    	if (left_finger_touching){
    		this->CreateFirstJoint();
    		this->grabPhase = 1;
    	}
    }

    if (this->grabPhase == 1){
    	if (right_fingers_touching){
    		this->BreakJoint();
    		this->CreateSecondJoints();
    		this->grabPhase = 2;
    	}
    }
}

void TiltGrabPlugin::Reset() {
    if (this->joint1 != nullptr) {
        this->BreakJoint();
    }
    if (this->joint2 != nullptr) {
    	this->joint2->Detach();
    	this->joint2 = nullptr;
    	this->joint3->Detach();
    	this->joint3 = nullptr;
    
    	// Enable gravity on the childLink
    	this->parentLink->SetGravityMode(true);

    	event::Events::DisconnectWorldUpdateBegin(this->updateConnection);
    	this->updateConnection = nullptr;
    }
}

void TiltGrabPlugin::CreateFirstJoint() {
    this->joint1 = this->physics->CreateJoint("fixed", this->model);
    // Bullet physics needs accurate joint position
    // ODE does't care
    this->joint1->Load(this->parentLink, this->childLink1, this->parentLink->GetWorldPose() - this->childLink1->GetWorldPose());
    this->joint1->Init();
    this->joint1->SetProvideFeedback(true);
    this->joint1->SetName("tilt_joint1_" + this->parentLink->GetScopedName() + "_" + this->childLink1->GetScopedName());
    
    // Disable gravity on the butter link
    this->parentLink->SetGravityMode(false);
    this->grabPhase = 1;

    this->updateConnection = event::Events::ConnectWorldUpdateBegin(
            boost::bind(&TiltGrabPlugin::OnUpdate, this, _1));
}

void TiltGrabPlugin::CreateSecondJoints() {
    this->joint2 = this->physics->CreateJoint("fixed", this->model);
    // Bullet physics needs accurate joint position
    // ODE does't care
    this->joint2->Load(this->parentLink, this->childLink2, this->parentLink->GetWorldPose() - this->childLink2->GetWorldPose());
    this->joint2->Init();
    this->joint2->SetProvideFeedback(true);
    this->joint2->SetName("grab_joint2_" + this->parentLink->GetScopedName() + "_" + this->childLink2->GetScopedName());

    this->joint3 = this->physics->CreateJoint("fixed", this->model);
    // Bullet physics needs accurate joint position
    // ODE does't care
    this->joint3->Load(this->parentLink, this->childLink3, this->parentLink->GetWorldPose() - this->childLink3->GetWorldPose());
    this->joint3->Init();
    this->joint3->SetProvideFeedback(true);
    this->joint3->SetName("grab_joint3_" + this->parentLink->GetScopedName() + "_" + this->childLink3->GetScopedName());
    
    // Disable gravity on the butter link
    this->parentLink->SetGravityMode(false);
    this->grabPhase = 2;

    this->updateConnection = event::Events::ConnectWorldUpdateBegin(
            boost::bind(&TiltGrabPlugin::OnUpdate, this, _1));
}

void TiltGrabPlugin::BreakJoint() {
    this->joint1->Detach();
    this->joint1 = nullptr;
    
    // Enable gravity on the childLink
    this->parentLink->SetGravityMode(true);

    event::Events::DisconnectWorldUpdateBegin(this->updateConnection);
    this->updateConnection = nullptr;
}
