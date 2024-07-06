/*
 * FileName:SMC_JXC51.h
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
    unsigned char jxc51_init(void);
    unsigned char jxc51_return(void);
    
    unsigned char jxc51_A_position(void);
    unsigned char jxc51_A_holdRelease(void);
    unsigned char jxc51_A_goto(unsigned char step_no);
    unsigned char jxc51_A_checkSTEP(void);
    unsigned char jxc51_A_checkSETUP(void);

#ifdef __USE_JXC51_B__
    unsigned char jxc51_B_position(void);
    unsigned char jxc51_B_holdRelease(void);
    unsigned char jxc51_B_goto(unsigned char step_no);
    unsigned char jxc51_B_checkSTEP(void);
    unsigned char jxc51_B_checkSETUP(void);
#endif

    /* @name   -taskJXC51
     * @brief  -This function is the whole control of all other function, therefore it should be called periodicity.
               Recommended period is not less than 20hz(50ms between each call).
               Stop calling this function will disable most function, but unexpected error could appear.
    */
    void taskJXC51(void);

#ifdef __cplusplus
}
#endif

#endif /* SMC_JXC51_H */
