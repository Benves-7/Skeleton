//
// Created by benves-7 on 9/25/19.
//

#ifndef SKELETON_H
#define SKELETON_H

#include "tinyxml.h"

#ifndef XMLCheckResult
#define XMLCheckResult(a_eResult) if (a_eResult != XML_SUCCESS) { printf("Error: %i\n", a_eResult); }
#endif


struct Joint
{
    std::string name;
    bool changed = false;
    int index, parent;
    Vector4D position, rotation, scale;
    Matrix4D mPos, mRot, mSca;
    Matrix4D transform, worldspaceTransform;
};



class Skeleton
        {
public:
    inline void loadSkeleton(const char* filename)
    {
        TiXmlDocument doc(filename);
        if (!doc.LoadFile())
            printf("Failed to load file \"%s\"\n", filename);

        TiXmlElement *eJoint = doc.FirstChildElement("Nebula3")->FirstChildElement("Model")->FirstChildElement("CharacterNodes")->FirstChildElement("CharacterNode")->FirstChildElement("Joint");
        while (eJoint)
        {
            Joint joint;
            joint.name = eJoint->Attribute("name");
            sscanf(eJoint->Attribute("index"), "%d", &joint.index);
            sscanf(eJoint->Attribute("parent"), "%d", &joint.parent);
            float temp[4];
            sscanf(eJoint->Attribute("position"), "%f,%f,%f,%f", &temp[0], &temp[1], &temp[2], &temp[3]);
            joint.position = Vector4D(temp[0],temp[1],temp[2],temp[3]);
            sscanf(eJoint->Attribute("rotation"), "%f,%f,%f,%f", &temp[0], &temp[1], &temp[2], &temp[3]);
            joint.rotation = Vector4D(temp[0],temp[1],temp[2],temp[3]);
            sscanf(eJoint->Attribute("scale"),    "%f,%f,%f,%f", &temp[0], &temp[1], &temp[2], &temp[3]);
            joint.scale    = Vector4D(temp[0],temp[1],temp[2],temp[3]);

            joint.mPos.GetPositionMatrix(joint.position);
            joint.mSca.GetScaleMatrix(joint.scale);
            joint.mRot.GetRotationFromQuaternian(joint.rotation);

            joint.transform = joint.mPos * joint.mRot * joint.mSca;

            joints.push_back(joint);
            eJoint = eJoint->NextSiblingElement();
        }
        for (int i = 0; i < joints.size(); ++i)
        {
            if (joints[i].parent != -1)
                joints[i].worldspaceTransform = joints[joints[i].parent].worldspaceTransform * joints[i].transform;
            else
                joints[i].worldspaceTransform = joints[i].transform;
        }
        TiXmlElement* joint_list;
    }
    inline void translateJoint(int jointIndex, Vector4D translation)
    {
        // moves first joint.
        Matrix4D temp; temp.GetPositionMatrix(translation);
        joints[jointIndex].worldspaceTransform = joints[joints[jointIndex].parent].worldspaceTransform * joints[jointIndex].transform * temp;
        joints[jointIndex].changed = true;
    }
    inline void rotateJoint(int jointIndex, Vector4D rotation)
    {
        // moves first joint.
        Matrix4D temp; temp.GetRotationFromQuaternian(rotation);
        joints[jointIndex].worldspaceTransform = joints[joints[jointIndex].parent].worldspaceTransform * joints[jointIndex].transform * temp;
        joints[jointIndex].changed = true;
    }
    inline void update()
    {

        for (int i = 0; i < joints.size(); ++i)
        {
            if (joints[i].parent != -1 && !joints[i].changed)
                joints[i].worldspaceTransform = joints[joints[i].parent].worldspaceTransform * joints[i].transform;
        }
    }
    vector<Joint> joints;
};


#endif //SKELETON_H
