#ifndef _ROS_GCRobotics_command_state_h
#define _ROS_GCRobotics_command_state_h

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "ros/msg.h"

namespace GCRobotics
{

  class command_state : public ros::Msg
  {
    public:
      int32_t state;
      bool mask_odom;

    virtual int serialize(unsigned char *outbuffer) const
    {
      int offset = 0;
      union {
        int32_t real;
        uint32_t base;
      } u_state;
      u_state.real = this->state;
      *(outbuffer + offset + 0) = (u_state.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_state.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_state.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_state.base >> (8 * 3)) & 0xFF;
      offset += sizeof(this->state);
      union {
        bool real;
        uint8_t base;
      } u_mask_odom;
      u_mask_odom.real = this->mask_odom;
      *(outbuffer + offset + 0) = (u_mask_odom.base >> (8 * 0)) & 0xFF;
      offset += sizeof(this->mask_odom);
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer)
    {
      int offset = 0;
      union {
        int32_t real;
        uint32_t base;
      } u_state;
      u_state.base = 0;
      u_state.base |= ((uint32_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_state.base |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_state.base |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_state.base |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      this->state = u_state.real;
      offset += sizeof(this->state);
      union {
        bool real;
        uint8_t base;
      } u_mask_odom;
      u_mask_odom.base = 0;
      u_mask_odom.base |= ((uint8_t) (*(inbuffer + offset + 0))) << (8 * 0);
      this->mask_odom = u_mask_odom.real;
      offset += sizeof(this->mask_odom);
     return offset;
    }

    const char * getType(){ return "GCRobotics/command_state"; };
    const char * getMD5(){ return "d27b4cb082b2e43166c03ade3dc51539"; };

  };

}
#endif