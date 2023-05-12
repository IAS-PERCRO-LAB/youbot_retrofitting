/******************************************************************************
* BRICS_3D - 3D Perception and Modeling Library
* Copyright (c) 2011, GPS GmbH
*
* Author: Sebastian Blumenthal
*
*
* This software is published under a dual-license: GNU Lesser General Public
* License LGPL 2.1 and Modified BSD license. The dual-license implies that
* users of this code may choose which terms they prefer.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU Lesser General Public License LGPL and the BSD license for
* more details.
*
******************************************************************************/

//ROS specific headers
#include <ros/ros.h>
#include <tf/transform_listener.h>
#include <tower_of_hanoi_sdk/GetSceneObjects.h>

//BRICS_3D specific headers
#include "brics_3d/worldModel/WorldModel.h"
#include "brics_3d/worldModel/sceneGraph/Box.h"
#include "brics_3d/core/HomogeneousMatrix44.h"
#include "brics_3d/core/Logger.h"

//System headers
#include <sstream>

namespace youBot {

class YouBotWorldModel {
public:

	YouBotWorldModel(ros::NodeHandle n) :
		node(n) {
		initialize();
	}

	virtual ~YouBotWorldModel() {

	}

	void initialize () {
		rootFrameId = "/openni_rgb_optical_frame";
		associationDistanceTreshold = 0.02; // [m]

		getObjectsService = node.advertiseService("youbot_3d_world_model/getSceneObjects", &YouBotWorldModel::onGetSceneObjects, this);
//		brics_3d::Logger::setMinLoglevel(brics_3d::Logger::LOGDEBUG);

		startFrameId = "start";
//		auxiliaryFrameId = "auxiliary";
		auxiliaryFrameId = "auxillary";
		goalFrameId = "goal";
		redBoxFrameId = "red_object_";
		greenBoxFrameId = "green_object_";
		yellowBoxFrameId = "yellow_object_";


		objectClasses.clear();
		vector<brics_3d::rsg::Attribute> objectAttributes;

		objectAttributes.clear();
		objectAttributes.push_back(brics_3d::rsg::Attribute("shapeType","Box"));
		objectAttributes.push_back(brics_3d::rsg::Attribute("taskType","targetArea"));
		objectAttributes.push_back(brics_3d::rsg::Attribute("name","start"));
		objectClasses.insert(std::make_pair(startFrameId, objectAttributes));

		objectAttributes.clear();
		objectAttributes.push_back(brics_3d::rsg::Attribute("shapeType","Box"));
		objectAttributes.push_back(brics_3d::rsg::Attribute("taskType","targetArea"));
		objectAttributes.push_back(brics_3d::rsg::Attribute("name","auxiliary"));
		objectClasses.insert(std::make_pair(auxiliaryFrameId, objectAttributes));

		objectAttributes.clear();
		objectAttributes.push_back(brics_3d::rsg::Attribute("shapeType","Box"));
		objectAttributes.push_back(brics_3d::rsg::Attribute("taskType","targetArea"));
		objectAttributes.push_back(brics_3d::rsg::Attribute("name","goal"));
		objectClasses.insert(std::make_pair(goalFrameId, objectAttributes));


		objectAttributes.clear();
		objectAttributes.push_back(brics_3d::rsg::Attribute("shapeType","Box"));
		objectAttributes.push_back(brics_3d::rsg::Attribute("color","red"));
		objectClasses.insert(std::make_pair(redBoxFrameId + "1", objectAttributes));
		objectClasses.insert(std::make_pair(redBoxFrameId + "2", objectAttributes));
		objectClasses.insert(std::make_pair(redBoxFrameId + "3", objectAttributes));

		objectAttributes.clear();
		objectAttributes.push_back(brics_3d::rsg::Attribute("shapeType","Box"));
		objectAttributes.push_back(brics_3d::rsg::Attribute("color","green"));
		objectClasses.insert(std::make_pair(greenBoxFrameId + "1", objectAttributes));
		objectClasses.insert(std::make_pair(greenBoxFrameId + "2", objectAttributes));
		objectClasses.insert(std::make_pair(greenBoxFrameId + "3", objectAttributes));

		objectAttributes.clear();
		objectAttributes.push_back(brics_3d::rsg::Attribute("shapeType","Box"));
		objectAttributes.push_back(brics_3d::rsg::Attribute("color","yellow"));
		objectClasses.insert(std::make_pair(yellowBoxFrameId + "1", objectAttributes));
		objectClasses.insert(std::make_pair(yellowBoxFrameId + "2", objectAttributes));
		objectClasses.insert(std::make_pair(yellowBoxFrameId + "3", objectAttributes));

	}

	bool onGetSceneObjects (tower_of_hanoi_sdk::GetSceneObjects::Request &req,
	           tower_of_hanoi_sdk::GetSceneObjects::Response &res) {

		/* parse request */
		ROS_DEBUG("Receiving new query.");
		vector<brics_3d::rsg::Attribute> queryAttributes;
		for (unsigned int i = 0; i < static_cast<unsigned int>(req.attributes.size()); ++i) {
			queryAttributes.push_back(brics_3d::rsg::Attribute(req.attributes[i].key ,req.attributes[i].value));
		}

		/* query */
		vector<brics_3d::SceneObject> resultObjects;
		myWM.getSceneObjects(queryAttributes, resultObjects);

		/* setup response */
		res.results.resize(resultObjects.size());
		geometry_msgs::Quaternion tmpQuaternion;
		geometry_msgs::TransformStamped tmpTransformMsg;
		std::stringstream objectSceneFrameID;
		for (unsigned int i = 0; i < static_cast<unsigned int>(resultObjects.size()); ++i) {
			tower_of_hanoi_sdk::SceneObject tmpSceneObject;
			tmpSceneObject.id = resultObjects[i].id;
			tmpSceneObject.parentId = resultObjects[i].parentId;

			tf::Transform tmpTransform;
			homogeniousMatrixToTfTransform(resultObjects[i].transform, tmpTransform);
			tmpTransformMsg.header.stamp = ros::Time::now();
			tmpTransformMsg.header.frame_id = rootFrameId;
			objectSceneFrameID.str("");
			objectSceneFrameID << "scene_object_" << resultObjects[i].id;
			tmpTransformMsg.child_frame_id = objectSceneFrameID.str();
			tmpTransformMsg.transform.translation.x = tmpTransform.getOrigin().getX();
			tmpTransformMsg.transform.translation.y = tmpTransform.getOrigin().getY();
			tmpTransformMsg.transform.translation.z = tmpTransform.getOrigin().getZ();
			tmpTransformMsg.transform.rotation.x = tmpTransform.getRotation().getX();
			tmpTransformMsg.transform.rotation.y = tmpTransform.getRotation().getY();
			tmpTransformMsg.transform.rotation.z = tmpTransform.getRotation().getZ();
			tmpTransformMsg.transform.rotation.w = tmpTransform.getRotation().getW();

			tmpSceneObject.transform = tmpTransformMsg;

			brics_3d::rsg::Box::BoxPtr tmpBox = boost::dynamic_pointer_cast<brics_3d::rsg::Box>(resultObjects[i].shape);

			if ( tmpBox != 0) {
				tmpSceneObject.shape.type = arm_navigation_msgs::Shape::BOX; //TODO support multible shapes
				tmpSceneObject.shape.dimensions.resize(3);
				tmpSceneObject.shape.dimensions[0] = tmpBox->getSizeX();
				tmpSceneObject.shape.dimensions[1] = tmpBox->getSizeY();
				tmpSceneObject.shape.dimensions[2] = tmpBox->getSizeZ();
			}


			tmpSceneObject.attributes.resize(resultObjects[i].attributes.size());
			for (unsigned int j = 0; j < static_cast<unsigned int>(resultObjects[i].attributes.size()); ++j) {
				tmpSceneObject.attributes[j].key = resultObjects[i].attributes[j].key;
				tmpSceneObject.attributes[j].value = resultObjects[i].attributes[j].value;
			}

			res.results[i] = tmpSceneObject;
		}


		return true;
	}

	void processTfTopic () {
		map <string, vector<brics_3d::rsg::Attribute> >::iterator iter = objectClasses.begin();
		for (iter = objectClasses.begin(); iter != objectClasses.end(); iter++) {

			string objectFrameId = iter->first;
			tf::StampedTransform transform;
			try{
				tfListener.lookupTransform(rootFrameId, objectFrameId, ros::Time(0), transform);
			}
			catch (tf::TransformException ex){
				ROS_WARN("%s",ex.what());
				continue;
			}

			if ( (ros::Time::now() - transform.stamp_) > maxTFCacheDuration ) { //simply ignore outdated TF frames
				ROS_WARN("TF found for %s. But it is outdated. Skipping it.", iter->first.c_str());
				continue;
			}
			ROS_INFO("TF found for %s.", iter->first.c_str());

			/* query */
			vector<brics_3d::rsg::Attribute> queryAttributes;;
			queryAttributes = iter->second;
			vector<brics_3d::SceneObject> resultObjects;

			myWM.getSceneObjects(queryAttributes, resultObjects);
//			ROS_INFO("Number of boxes: %i " , static_cast<unsigned int>(resultObjects.size()));

			/* associate */
			unsigned int index = -1;
			double minSquardDistanceToExistingObjects = std::numeric_limits<double>::max();
			const double* matrixPtr;

			for (unsigned int i = 0; i < static_cast<unsigned int>(resultObjects.size()) ; ++i) {
				matrixPtr = resultObjects[i].transform->getRawData();
				double squardDistanceToExistingObjects;
				double xPercieved = transform.getOrigin().x();
				double yPercieved = transform.getOrigin().y();
				double zPercieved = transform.getOrigin().z();
				double xStored = matrixPtr[12];
				double yStored = matrixPtr[13];
				double zStored = matrixPtr[14];

				squardDistanceToExistingObjects = 	(xPercieved - xStored) * (xPercieved - xStored) +
						(yPercieved - yStored) * (yPercieved - yStored) +
						(zPercieved - zStored) * (zPercieved - zStored);

				if (squardDistanceToExistingObjects < minSquardDistanceToExistingObjects) {
					minSquardDistanceToExistingObjects = squardDistanceToExistingObjects;
					index = i;
				}
			}

			ROS_INFO("Shortest distance %lf to found result object %i.", minSquardDistanceToExistingObjects, index);

			if (minSquardDistanceToExistingObjects < (associationDistanceTreshold * associationDistanceTreshold) ) {

				/* update existing */
				ROS_INFO("Updating existing scene object with object ID: %i", resultObjects[index].id);
				brics_3d::IHomogeneousMatrix44::IHomogeneousMatrix44Ptr newTransform(new brics_3d::HomogeneousMatrix44());
				tfTransformToHomogeniousMatrix(transform, newTransform);
				myWM.insertTransform(resultObjects[index].id, newTransform);

			} else {

				/* insert */
				ROS_INFO("Inserting new scene object");
				brics_3d::rsg::Shape::ShapePtr boxShape(new brics_3d::rsg::Box(cubeSize, cubeSize, cubeSize)); // in [m]
				brics_3d::rsg::Shape::ShapePtr targetAreaBoxShape(new brics_3d::rsg::Box(targetAreaSizeX, targetAreaSizeY, targetAreaSizeZ)); // in [m]
				brics_3d::IHomogeneousMatrix44::IHomogeneousMatrix44Ptr initialTransform(new brics_3d::HomogeneousMatrix44());
				tfTransformToHomogeniousMatrix(transform, initialTransform);
				brics_3d::SceneObject tmpSceneObject;
				if ( (iter->first.compare(startFrameId) == 0) || (iter->first.compare(auxiliaryFrameId) == 0) || (iter->first.compare(goalFrameId) == 0)) {
					tmpSceneObject.shape = targetAreaBoxShape;
				} else {
					tmpSceneObject.shape = boxShape;
				}

//				tmpSceneObject.shape = boxShape;
				tmpSceneObject.transform = initialTransform;
				tmpSceneObject.parentId =  myWM.getRootNodeId(); // hook in after root node
				tmpSceneObject.attributes.clear();
				tmpSceneObject.attributes = iter->second;

				unsigned int returnedId;
				myWM.addSceneObject(tmpSceneObject, returnedId);
			}

		}

		/* query */
		vector<brics_3d::rsg::Attribute> queryAttributes;
		vector<brics_3d::SceneObject> resultObjects;
		queryAttributes.push_back(brics_3d::rsg::Attribute("shapeType","Box"));
//		queryAttributes.push_back(brics_3d::rsg::Attribute("color","red"));

		myWM.getSceneObjects(queryAttributes, resultObjects);
		ROS_INFO("Total number of boxes: %i " , static_cast<unsigned int>(resultObjects.size()));

	}

	void cleanUpWorldModelData() {
		myWM.runOncePerception();
	}

	/* Some helper functions */
	void tfTransformToHomogeniousMatrix (const tf::Transform& tfTransform, brics_3d::IHomogeneousMatrix44::IHomogeneousMatrix44Ptr& transformMatrix)
	{
		double mv[12];

		tfTransform.getBasis().getOpenGLSubMatrix(mv);
		tf::Vector3 origin = tfTransform.getOrigin();

		double* matrixPtr = transformMatrix->setRawData();

		/* matrices are column-major */
		matrixPtr[0] = mv[0]; matrixPtr[4] = mv[4]; matrixPtr[8] = mv[8];   matrixPtr[12] = origin.x();
		matrixPtr[1] = mv[1]; matrixPtr[5] = mv[5]; matrixPtr[9] = mv[9];   matrixPtr[13] = origin.y();
		matrixPtr[2] = mv[2]; matrixPtr[6] = mv[6]; matrixPtr[10] = mv[10]; matrixPtr[14] = origin.z();
		matrixPtr[3] = 1;     matrixPtr[7] = 1;     matrixPtr[11] = 1;      matrixPtr[15] = 1;

	}

	void homogeniousMatrixToTfTransform (const brics_3d::IHomogeneousMatrix44::IHomogeneousMatrix44Ptr& transformMatrix, tf::Transform& tfTransform) {
		const double* matrixPtr = transformMatrix->getRawData();

		btVector3 translation;
		btMatrix3x3 rotation;

		translation.setX(matrixPtr[12]);
		translation.setY(matrixPtr[13]);
		translation.setZ(matrixPtr[14]);

		rotation.setValue(
				matrixPtr[0], matrixPtr[4], matrixPtr[8],
				matrixPtr[1], matrixPtr[5], matrixPtr[9],
				matrixPtr[2], matrixPtr[6], matrixPtr[10]
		);

		tfTransform.setOrigin(translation);
		tfTransform.setBasis(rotation);
	}

	/// To this TF frame_id all objects in the world model relate. Default is the Kinect frame.
	string rootFrameId;

	/// Minimal distance the needs to be exceeded to associate a perceived object with an already seen/stored one.
	double associationDistanceTreshold;

	/// The x,y,z sizes of the cubes to be grasped.
	double cubeSize;

	double targetAreaSizeX;
	double targetAreaSizeY;
	double targetAreaSizeZ;

	ros::Duration maxTFCacheDuration;

private:

	/// The ROS node handle
	ros::NodeHandle node;

	/// Handle for the 3D world model
	brics_3d::WorldModel myWM;

	/// Receives TF
	tf::TransformListener tfListener;

	/// Provided service to get access to the stored data
	ros::ServiceServer getObjectsService;

	/* hanoi specifc IDs */
	string startFrameId;
	string auxiliaryFrameId;
	string goalFrameId;

	string redBoxFrameId;
	string greenBoxFrameId;
	string yellowBoxFrameId;

	/// Mapping that assignes attributes to relevant TF frame_ids
	map <string, vector<brics_3d::rsg::Attribute> > objectClasses;

};


}  // namespace youBot


int main(int argc, char **argv)
{

	brics_3d::Logger::setMinLoglevel(brics_3d::Logger::LOGDEBUG);
	ros::init(argc, argv, "youbot_3d_world_model");
	ros::NodeHandle n;
	youBot::YouBotWorldModel youbotWM(n);


	/* configuration */
	n.param<std::string>("worldModelRootFrameId", youbotWM.rootFrameId, "/openni_rgb_optical_frame");
	n.param<double>("worldModelcubeSize", youbotWM.cubeSize, 0.048);
	// (0.38, 0.13, 0.15) This is approx. a Kinect box...
	n.param<double>("worldModelTargetAreaSizeX", youbotWM.targetAreaSizeX, 0.15);
	n.param<double>("worldModelTargetAreaSizeY", youbotWM.targetAreaSizeY, 0.38);
	n.param<double>("worldModelTargetAreaSizeZ", youbotWM.targetAreaSizeZ, 0.13);
	n.param<double>("worldModelassociationDistanceTreshold", youbotWM.associationDistanceTreshold, 0.02);

	youbotWM.maxTFCacheDuration = ros::Duration(1.0); //[s]
	/* coordination */
//	ros::spin();

	ros::Rate rate(10); // (in Hz)
	while (n.ok()){
		ros::spinOnce();
		youbotWM.processTfTopic();
		youbotWM.cleanUpWorldModelData();
		rate.sleep();
	}

	return 0;
}

/* EOF */
