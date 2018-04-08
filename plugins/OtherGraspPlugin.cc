#include "OtherGraspPlugin.hh"

#include <ros/ros.h>
#include <gazebo/physics/physics.hh>
#include <string>
#include <gazebo/sensors/sensors.hh>

using namespace gazebo;
GZ_REGISTER_SENSOR_PLUGIN(OtherGraspPlugin)

/////////////////////////////////////////////////
OtherGraspPlugin::OtherGraspPlugin() : SensorPlugin()
{
}

/////////////////////////////////////////////////
OtherGraspPlugin::~OtherGraspPlugin()
{
}

/////////////////////////////////////////////////
void OtherGraspPlugin::Load(sensors::SensorPtr _sensor, sdf::ElementPtr _sdf)
{
  //std::cout << "initialised graspingplugin \n";
  ROS_INFO("Hello World!");
  // Get the parent sensor.
  this->parentSensor =
    std::dynamic_pointer_cast<sensors::ContactSensor>(_sensor);

  const std::string childLinkName1 = _sdf->GetElement("childLinkName1")->Get<std::string>();
  const std::string childLinkName2 = _sdf->GetElement("childLinkName2")->Get<std::string>();
  const std::string childLinkName3 = _sdf->GetElement("childLinkName3")->Get<std::string>();
  const std::string parentLinkName = _sdf->GetElement("parentLinkName")->Get<std::string>();
  const std::string SensorName = _sdf->GetElement("sensorName")->Get<std::string>();

  // Make sure the parent sensor is valid.
  if (!this->parentSensor)
  {
    gzerr << "ContactPlugin requires a ContactSensor.\n";
    return;
  }

  // Connect to the sensor update event.
  this->updateConnection = this->parentSensor->ConnectUpdated(
      std::bind(&OtherGraspPlugin::OnUpdate, this));

  // Make sure the parent sensor is active.
  this->parentSensor->SetActive(true);

  std::cout << "initialised graspingplugin \n";
  gzdbg << "message" << std::endl;
}

/////////////////////////////////////////////////
void OtherGraspPlugin::OnUpdate()
{
  // Get all the contacts.
  msgs::Contacts contacts;
  contacts = this->parentSensor->Contacts();
  for (unsigned int i = 0; i < contacts.contact_size(); ++i)
  {
    std::cout << "Collision between[" << contacts.contact(i).collision1()
              << "] and [" << contacts.contact(i).collision2() << "]\n";

    for (unsigned int j = 0; j < contacts.contact(i).position_size(); ++j)
    {
      std::cout << j << "  Position:"
                << contacts.contact(i).position(j).x() << " "
                << contacts.contact(i).position(j).y() << " "
                << contacts.contact(i).position(j).z() << "\n";
      std::cout << "   Normal:"
                << contacts.contact(i).normal(j).x() << " "
                << contacts.contact(i).normal(j).y() << " "
                << contacts.contact(i).normal(j).z() << "\n";
      std::cout << "   Depth:" << contacts.contact(i).depth(j) << "\n";
    }
  }
}