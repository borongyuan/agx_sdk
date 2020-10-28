/* 
 * tracer_base.hpp
 * 
 * Created on: Apr 14, 2020 10:21
 * Description: 
 * 
 * Copyright (c) 2020 Ruixiang Du (rdu)
 */ 

#ifndef TRACER_BASE_HPP
#define TRACER_BASE_HPP

#include <string>
#include <cstdint>
#include <thread>
#include <mutex>


//#include "wrp_sdk/asyncio/async_can.hpp"
//#include "wrp_sdk/asyncio/async_serial.hpp"
#include "wrp_sdk/platforms/common/mobile_base.hpp"

#include "wrp_sdk/platforms/tracer/tracer_protocol.h"
#include "wrp_sdk/platforms/tracer/tracer_can_parser.h"
#include "wrp_sdk/platforms/tracer/tracer_types.hpp"

namespace westonrobot
{
class TracerBase :public MobileBase
{
public:
    TracerBase() : MobileBase(){};
    ~TracerBase()= default;

    // get robot state
    TracerState GetTracerState();

    // motion control
    void SetMotionCommand(double linear_vel, double angular_vel,
                          TracerMotionCmd::FaultClearFlag fault_clr_flag = TracerMotionCmd::FaultClearFlag::NO_FAULT);

    // light control
    void SetLightCommand(TracerLightCmd cmd);
    void DisableLightCmdControl();



private:

    // CAN priority higher than serial if both connected
//    bool can_connected_ = false;
//    bool serial_connected_ = false;

    // serial port related variables
//    uint8_t tx_cmd_len_;
//    uint8_t tx_buffer_[TRACER_CMD_BUF_LEN];

    // cmd/status update related variables
    //std::thread cmd_thread_;
    std::mutex tracer_state_mutex_;
    std::mutex motion_cmd_mutex_;
    std::mutex light_cmd_mutex_;
    std::mutex mode_cmd_mutex_;

    TracerState tracer_state_;
    TracerMotionCmd current_motion_cmd_;
    TracerLightCmd current_light_cmd_;

//    int32_t cmd_thread_period_ms_ = 10;
//    bool cmd_thread_started_ = false;

    bool light_ctrl_enabled_ = false;
    bool light_ctrl_requested_ = false;

    void SendMotionCmd(uint8_t count);
    void SendRobotCmd() override;
    void SendLightCmd(uint8_t count);
    void SendControlCmd();
    void ParseCANFrame(can_frame *rx_frame) override;
    void ParseUARTBuffer(uint8_t *buf, const size_t bufsize, size_t bytes_received) override{};

    void NewStatusMsgReceivedCallback(const TracerMessage &msg);

public:
    static void UpdateTracerState(const TracerMessage &status_msg, TracerState &state);
};
} // namespace westonrobot

#endif /* TRACER_BASE_HPP */
