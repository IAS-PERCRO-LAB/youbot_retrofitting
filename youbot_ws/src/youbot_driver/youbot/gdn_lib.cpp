#include <cppunit/TestCase.h>
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/CompilerOutputter.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/ui/text/TestRunner.h>
#include <cppunit/extensions/HelperMacros.h>

#include <iostream>
#include <vector>

#include <signal.h>

#include "YouBotBase.hpp"
#include "YouBotManipulator.hpp"
#include "YouBotGripper.hpp"
#include "DataTrace.hpp"

youbot::YouBotManipulator *YouBotManipulator = NULL;

static int64_t wakes = -1;
static int64_t wakeus = -1;

struct timeval export_time;

extern "C"
{
    void Init_Manipulator(int connect)
    {
      std::cout << "Attention! All joint of the youBot will move during the test. \nThe youBot should NOT stand on the ground and the wheels should be in the air! \nAlso the arm will move please be carefull!" << std::endl;
      char input = 0;      
      while (input != 'y' && input != 'n')
      {
        std::cout << "Are all joint off the ground? [n/y]" << std::endl;
        if (input == 'n')
           {
             std::cout << "stop run" << std::endl;
           }
        else
           { 
           std::cout << "Ok running" << std::endl;
           // does the sine commutation of the base joints ->doJointCommutation
           if (connect == 1)
              {
               std::cout << "try inizialize YouBot arm" << std::endl;
               YouBotManipulator = new youbot::YouBotManipulator("youbot-manipulator", "/home/youbot/fuerte_workspace/sandbox/youbot_driver/config");
               if (YouBotManipulator !=NULL)
                  { 
                   std::cout << "try doJointCommutation" << std::endl;
                   YouBotManipulator -> doJointCommutation();
                   std::cout << "try calibration" <<std::endl;
                   YouBotManipulator -> calibrateManipulator();
                   YouBotManipulator -> calibrateGripper();
                  }
                else
                    {
                     std::cout << "not try inizialize try doJointCommutation" << std::endl;
                     input = 'n';
                    }
              }
           else
               {
                std::cout << "not try inizialize youbotmanipulator" << std::endl;
               }
           }
        input = 'n';
      }
    }

    int64_t stamp_time(void)
    {     
     gettimeofday(&export_time,NULL);
     if (wakes == -1) wakes = export_time.tv_sec;
     if (wakeus == -1) wakeus = export_time.tv_usec;
     return 1000000*(export_time.tv_sec-wakes)+export_time.tv_usec-wakeus;
    }    
    
    void SIMULINKsync(long microsec)
    {
     SLEEP_MICROSEC(microsec);
    }
    
    void Stop_Manipulator(int Nconnect)
    {
      std::cout << "stop arm" << std::endl;
      YouBotManipulator -> ~YouBotManipulator();
      if (YouBotManipulator != NULL)
         {
          delete YouBotManipulator;
         }
    }
    
    void SetCurrent_Manipulator (double *setcurr)
    {
     std::vector<youbot::JointCurrentSetpoint> SetArmCur(5);
     SetArmCur[0]=setcurr[0]*si::ampere;
     SetArmCur[1]=setcurr[1]*si::ampere;
     SetArmCur[2]=setcurr[2]*si::ampere;
     SetArmCur[3]=setcurr[3]*si::ampere;
     SetArmCur[4]=setcurr[4]*si::ampere;
     if (YouBotManipulator != NULL)
         YouBotManipulator  -> setJointData(SetArmCur);       
    }

    void SetPosition_Manipulator (double *setpos)
    {
     std::vector<youbot::JointAngleSetpoint> SetArmPos(5);
     SetArmPos[0]=setpos[0]*si::radian;
     SetArmPos[1]=setpos[1]*si::radian;
     SetArmPos[2]=setpos[2]*si::radian;
     SetArmPos[3]=setpos[3]*si::radian;
     SetArmPos[4]=setpos[4]*si::radian;
     if (YouBotManipulator  != NULL)
         YouBotManipulator  -> setJointData(SetArmPos);       
    }

    void SetVelocity_Manipulator(double *setvel)
    {
     std::vector<youbot::JointVelocitySetpoint> SetArmVel(5);
     SetArmVel[0]=setvel[0]*si::radian_per_second;
     SetArmVel[1]=setvel[1]*si::radian_per_second;
     SetArmVel[2]=setvel[2]*si::radian_per_second;
     SetArmVel[3]=setvel[3]*si::radian_per_second;
     SetArmVel[4]=setvel[4]*si::radian_per_second;
     if (YouBotManipulator != NULL)
         YouBotManipulator -> setJointData(SetArmVel);       
    }

    void SetTorque_Manipulator(double *settor)
    {
     std::vector<youbot::JointTorqueSetpoint> SetArmTor(5);
     SetArmTor[0]=settor[0]*si::newton_meter;
     SetArmTor[1]=settor[1]*si::newton_meter;
     SetArmTor[2]=settor[2]*si::newton_meter;
     SetArmTor[3]=settor[3]*si::newton_meter;
     SetArmTor[4]=settor[4]*si::newton_meter;
     if (YouBotManipulator != NULL)
         YouBotManipulator -> setJointData(SetArmTor);       
    }

    double GetCurrent_Manipulator0(void)
    {
     std::vector<youbot::JointSensedCurrent> GetArmCur(5);
     double getcurr;
     if (YouBotManipulator != NULL)
        {
         YouBotManipulator -> getJointData(GetArmCur);
         getcurr=(GetArmCur[0].current)/(ampere);
        }
     return getcurr;
    }

    double GetCurrent_Manipulator1(void)
    {
     std::vector<youbot::JointSensedCurrent> GetArmCur(5);
     double getcurr;
     if (YouBotManipulator != NULL)
        {
         YouBotManipulator -> getJointData(GetArmCur);
         getcurr=(GetArmCur[1].current)/(ampere);
        }
     return getcurr;
    }

    double GetCurrent_Manipulator2(void)
    {
     std::vector<youbot::JointSensedCurrent> GetArmCur(5);
     double getcurr;
     if (YouBotManipulator != NULL)
        {
         YouBotManipulator -> getJointData(GetArmCur);
         getcurr=(GetArmCur[2].current)/(ampere);
        }
     return getcurr;
    }

    double GetCurrent_Manipulator3(void)
    {
     std::vector<youbot::JointSensedCurrent> GetArmCur(5);
     double getcurr;
     if (YouBotManipulator != NULL)
        {
         YouBotManipulator -> getJointData(GetArmCur);
         getcurr=(GetArmCur[3].current)/(ampere);
        }
     return getcurr;
    }
        
    double GetCurrent_Manipulator4(void)
    {
     std::vector<youbot::JointSensedCurrent> GetArmCur(5);
     double getcurr;
     if (YouBotManipulator != NULL)
        {
         YouBotManipulator -> getJointData(GetArmCur);
         getcurr=(GetArmCur[4].current)/(ampere);
        }
     return getcurr;
    }

    double GetPosition_Manipulator0(void)
    {
     std::vector<youbot::JointSensedAngle> GetArmPos(5);
     double getpos;
     if (YouBotManipulator != NULL)
        {
         YouBotManipulator -> getJointData(GetArmPos);
         getpos=(GetArmPos[0].angle)/(radian);
        }
     return getpos;
    } 

    double GetPosition_Manipulator1(void)
    {
     std::vector<youbot::JointSensedAngle> GetArmPos(5);
     double getpos;
     if (YouBotManipulator != NULL)
        {
         YouBotManipulator -> getJointData(GetArmPos);
         getpos=(GetArmPos[1].angle)/(radian);
        }
     return getpos;
    } 

    double GetPosition_Manipulator2(void)
    {
     std::vector<youbot::JointSensedAngle> GetArmPos(5);
     double getpos;
     if (YouBotManipulator != NULL)
        {
         YouBotManipulator -> getJointData(GetArmPos);
         getpos=(GetArmPos[2].angle)/(radian);
        }
     return getpos;
    } 

    double GetPosition_Manipulator3(void)
    {
     std::vector<youbot::JointSensedAngle> GetArmPos(5);
     double getpos;
     if (YouBotManipulator != NULL)
        {
         YouBotManipulator -> getJointData(GetArmPos);
         getpos=(GetArmPos[3].angle)/(radian);
        }
     return getpos;
    } 
    
    double GetPosition_Manipulator4(void)
    {
     std::vector<youbot::JointSensedAngle> GetArmPos(5);
     double getpos;
     if (YouBotManipulator != NULL)
        {
         YouBotManipulator -> getJointData(GetArmPos);
         getpos=(GetArmPos[4].angle)/(radian);
        }
     return getpos;
    } 

    double GetVelocity_Manipulator0(void)
    {
     std::vector<youbot::JointSensedVelocity> GetArmVel(5) ;
     double getvel;
     if (YouBotManipulator != NULL)
        {
         YouBotManipulator -> getJointData(GetArmVel);
         getvel=(GetArmVel[0].angularVelocity)/(radian_per_second);
        }
     return getvel;
    }

    double GetVelocity_Manipulator1(void)
    {
     std::vector<youbot::JointSensedVelocity> GetArmVel(5) ;
     double getvel;
     if (YouBotManipulator != NULL)
        {
         YouBotManipulator -> getJointData(GetArmVel);
         getvel=(GetArmVel[1].angularVelocity)/(radian_per_second);
        }
     return getvel;
    }

    double GetVelocity_Manipulator2(void)
    {
     std::vector<youbot::JointSensedVelocity> GetArmVel(5) ;
     double getvel;
     if (YouBotManipulator != NULL)
        {
         YouBotManipulator -> getJointData(GetArmVel);
         getvel=(GetArmVel[2].angularVelocity)/(radian_per_second);
        }
     return getvel;
    }

    double GetVelocity_Manipulator3(void)
    {
     std::vector<youbot::JointSensedVelocity> GetArmVel(5) ;
     double getvel;
     if (YouBotManipulator != NULL)
        {
         YouBotManipulator -> getJointData(GetArmVel);
         getvel=(GetArmVel[3].angularVelocity)/(radian_per_second);
        }
     return getvel;
    }
    
    double GetVelocity_Manipulator4(void)
    {
     std::vector<youbot::JointSensedVelocity> GetArmVel(5) ;
     double getvel;
     if (YouBotManipulator != NULL)
        {
         YouBotManipulator -> getJointData(GetArmVel);
         getvel=(GetArmVel[4].angularVelocity)/(radian_per_second);
        }
     return getvel;
    }

    double GetTorque_Manipulator0(void)
    {
     std::vector<youbot::JointSensedTorque> GetArmTor(5);
     double gettor;
     if (YouBotManipulator != NULL)
        {
         YouBotManipulator -> getJointData(GetArmTor);
         gettor=(GetArmTor[0].torque)/(newton_meter);
        }
     return gettor;
    }

    double GetTorque_Manipulator1(void)
    {
     std::vector<youbot::JointSensedTorque> GetArmTor(5);
     double gettor;
     if (YouBotManipulator != NULL)
        {
         YouBotManipulator -> getJointData(GetArmTor);
         gettor=(GetArmTor[1].torque)/(newton_meter);
        }
     return gettor;
    }

    double GetTorque_Manipulator2(void)
    {
     std::vector<youbot::JointSensedTorque> GetArmTor(5);
     double gettor;
     if (YouBotManipulator != NULL)
        {
         YouBotManipulator -> getJointData(GetArmTor);
         gettor=(GetArmTor[2].torque)/(newton_meter);
        }
     return gettor;
    }

    double GetTorque_Manipulator3(void)
    {
     std::vector<youbot::JointSensedTorque> GetArmTor(5);
     double gettor;
     if (YouBotManipulator != NULL)
        {
         YouBotManipulator -> getJointData(GetArmTor);
         gettor=(GetArmTor[3].torque)/(newton_meter);
        }
     return gettor;
    }
    
    double GetTorque_Manipulator4(void)
    {
     std::vector<youbot::JointSensedTorque> GetArmTor(5);
     double gettor;
     if (YouBotManipulator != NULL)
        {
         YouBotManipulator -> getJointData(GetArmTor);
         gettor=(GetArmTor[4].torque)/(newton_meter);
        }
     return gettor;
    }
  }
