/*
 * File:   LECA6_x2_16F1947_.h
 * Author: Behind Tommy
 *
 * Created on February 1, 2024, 9:20 AM
 */

#ifndef SMC_JXC51_H
#define SMC_JXC51_H

#define __USE_IO_EXPAND__
#define __USE_JXC51_B__

#ifdef __cplusplus
extern "C"
{
#endif
    /* @name    -taskJXC51
     * @brief   -This function is the whole control of all other function, therefore it should be called periodicity.
                too long or too short period will cause problem. Recommended period is 20hz(50ms between each call).
                Stop calling this function will disable most function, but unexpected error could appear.
    */
    void taskJXC51(void);
    
    /* 
     * @name    jxc51_init
     * @brief   init the program, then send setup request to the controller.
     * TODO: remove the setup progress from init, so user can decide when to setup instead of always setup at the very start.
     */
    uint8_t jxc51_init(void);

    /*
     * @name    jxc51_setup
     * @brief   setup the  controller(s), the actuator(s) will return to ORG.
     */
    uint8_t jxc51_setup(void);
    
    /*
     * @name    jxc51_check_setup
     * @brief   check if the controller(s) have ended setup.
     */
    uint8_t jxc51_check_setup(void);

    /* @name    jxc51_A_position
     * @brief   read the position from controller A (IN0, IN1, IN2 of parallel I/O).
     */
    uint8_t jxc51_A_position(void);

    /*
     * @name  jxc51_A_holdRelease
     * @brief  this function send a hold command or release command to the taskJXC51, then taskJXC51 control the controller A.
     */
    uint8_t jxc51_A_holdRelease(void);

    /*
     * @name jxc51_A_goto
     * @brief  this function send a move command to the taskJXC51, then taskJXC51 control the controller A.
     * @param
     */
    uint8_t jxc51_A_goto(unsigned char step_no);

    /*
     * @name  jxc51_A_check_step
     * @brief this function check if controller A have reached the target position, the reading from controller is done by the taskJXC51.
     */
    uint8_t jxc51_A_check_step(void);

    /*
     * @name jxc51_A_check_alarm
     * @brief this function check if controller A have any alarm, the reading from controller is done by the taskJXC51.
     * TODO: add support of alarm code
     */
    uint8_t jxc51_A_check_alarm(void);

#ifdef __USE_JXC51_B__
    /*
     * @name jxc51_B_position
     * @brief read the position from the controller B (IN0, IN1, IN2 of parallel I/O).
     */
    uint8_t jxc51_B_position(void);

    /*
     * @name jxc51_B_holdRelease
     * @brief same as jxc51_A_holdRelease, but for controller B.
     */
    uint8_t jxc51_B_holdRelease(void);

    /*
     * @name jxc51_B_goto
     * @brief same as jxc51_A_goto, but for controller B.
     */
    uint8_t jxc51_B_goto(unsigned char step_no);

    /*
     * @name jxc51_B_check_step
     * @brief same as jxc51_A_check_step, but for controller B.
     */
    uint8_t jxc51_B_check_step(void);
#endif


#ifdef __cplusplus
}
#endif

#endif /* LECA6_X2_16F1947__H */
