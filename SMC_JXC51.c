#include "SMC_JXC51.h"

#include "board_support.h"
#ifdef __USE_IO_EXPAND__
#include "I2C_interface.h"
#endif

#ifndef __USE_IO_EXPAND__
// JXC51_A_input
#define JXC51_A_IN0 OUT8
#define JXC51_A_IN1 OUT7
#define JXC51_A_IN2 OUT6
#define JXC51_A_SETUP OUT5
#define JXC51_A_HOLD OUT4
#define JXC51_A_DRIVE OUT3
#define JXC51_A_RESET OUT2
#define JXC51_A_SVON OUT1

#ifdef __USE_JXC51_B__
// JXC51_B_input
#define JXC51_B_IN0 OUT16
#define JXC51_B_IN1 OUT15
#define JXC51_B_IN2 OUT14
#define JXC51_B_SETUP OUT13
#define JXC51_B_HOLD OUT12
#define JXC51_B_DRIVE OUT11
#define JXC51_B_RESET OUT10
#define JXC51_B_SVON OUT9

#endif

/* the outputs have a '!' because my board have such design on it, please be aware */
// JXC51_A output
#define JXC51_A_OUT0 !IN1
#define JXC51_A_OUT1 !IN2
#define JXC51_A_OUT2 !IN3
#define JXC51_A_BUSY !IN4
#define JXC51_A_SETON !IN5
#define JXC51_A_INP !IN6
#define JXC51_A_SVRE !IN7
#define JXC51_A_ALARM !IN8

#ifdef __USE_JXC51_B__
//    JXC51_B output
#define JXC51_B_OUT0 !IN9
#define JXC51_B_OUT1 !IN10
#define JXC51_B_OUT2 !IN11
#define JXC51_B_BUSY !IN12
#define JXC51_B_SETON !IN13
#define JXC51_B_INP !IN14
#define JXC51_B_SVRE !IN15
#define JXC51_B_ALARM !IN16

#endif

#else
struct
{
    unsigned svon : 1;  // OUT1
    unsigned reset : 1; // OUT2
    unsigned drive : 1; // OUT3
    unsigned hold : 1;  // OUT4
    unsigned setup : 1; // OUT5
    unsigned in2 : 1;   // OUT6
    unsigned in1 : 1;   // OUT7
    unsigned in0 : 1;   // OUT8
} jxc51_expand_in;
unsigned char *jxc51_in_data = (unsigned char *)&jxc51_expand_in;

struct
{
    unsigned out0 : 1;  // IN1
    unsigned out1 : 1;  // IN2
    unsigned out2 : 1;  // IN3
    unsigned busy : 1;  // IN4
    unsigned seton : 1; // IN5
    unsigned inp : 1;   // IN6
    unsigned svre : 1;  // IN7
    unsigned alarm : 1; // IN8
} jxc51_expand_out;
unsigned char *jxc51_out_data = (unsigned char *)&jxc51_expand_out;

// JXC51_A_input
#define JXC51_A_IN0 jxc51_expand_in.in0     // OUT 8
#define JXC51_A_IN1 jxc51_expand_in.in1     // OUT 7
#define JXC51_A_IN2 jxc51_expand_in.in2     // OUT 6
#define JXC51_A_SETUP jxc51_expand_in.setup // OUT 5
#define JXC51_A_HOLD jxc51_expand_in.hold   // OUT 4
#define JXC51_A_DRIVE jxc51_expand_in.drive // OUT 3
#define JXC51_A_RESET jxc51_expand_in.reset // OUT 2
#define JXC51_A_SVON jxc51_expand_in.svon   // OUT 1

// #define JXC51_A_IN3 RD0 //START_OUTPUT
// #define JXC51_A_IN4 RD1 //FUNCTION_OUTPUT

#ifdef __USE_JXC51_B__
// JXC51_B_input (whitch is jxc_A when not using IO_EXPAND)
#define JXC51_B_IN0  OUT8
#define JXC51_B_IN1  OUT7
#define JXC51_B_IN2  OUT6
#define JXC51_B_SETUP OUT5
#define JXC51_B_HOLD  OUT4
#define JXC51_B_DRIVE OUT3
#define JXC51_B_RESET OUT2
#define JXC51_B_SVON  OUT1

#endif

// JXC51_A output
#define JXC51_A_OUT0 jxc51_expand_out.out0   // IN 1
#define JXC51_A_OUT1 jxc51_expand_out.out1   // IN 2
#define JXC51_A_OUT2 jxc51_expand_out.out2   // IN 3
#define JXC51_A_BUSY jxc51_expand_out.busy   // IN 4
#define JXC51_A_SETON jxc51_expand_out.seton // IN 5
#define JXC51_A_INP jxc51_expand_out.inp     // IN 6
#define JXC51_A_SVRE jxc51_expand_out.svre   // IN 7
#define JXC51_A_ALARM jxc51_expand_out.alarm // IN 8

// #define JXC51_A_OUT3 !RD5  //START_INPUT
// #define JXC51_A_OUT4 !RD6 //FUNCTION_INPUT

#ifdef __USE_JXC51_B__
//    JXC51_B output (whitch is jxc_A when not using IO_EXPAND)
#define JXC51_B_OUT0  IN1
#define JXC51_B_OUT1  IN2
#define JXC51_B_OUT2  IN3
#define JXC51_B_BUSY  IN4
#define JXC51_B_SETON IN5
#define JXC51_B_INP  IN6
#define JXC51_B_SVRE  IN7
#define JXC51_B_ALARM IN8

#endif

#endif

#define SETUP_REQ_NULL 1
#define STEP_REQ_NULL 0xFF
#define FLAG_START 1
#define FLAG_RESET 2
#define FLAG_RUN 3

static uint8_t jxc51_setup_finish_flag;
static uint8_t jxc51_A_stored_step;
#ifdef __USE_JXC51_B__
static uint8_t jxc51_B_stored_step;
#endif
static uint8_t jxc51_A_step_finish_flag;
#ifdef __USE_JXC51_B__
static uint8_t jxc51_B_step_finish_flag;
#endif

#ifdef __USE_IO_EXPAND__
void JXC51_A_APPLY(void)
{
    I2C_W(*jxc51_in_data, EXPAND_GPIOA, 0b110); // A2=1;A1=1;A0=0;
}

void JXC51_A_READ(void)
{
/* the outputs have a '~' because my board have such design on it, please be aware */
    *jxc51_out_data = ~(I2C_R(EXPAND_GPIOA, 0b111)); // A2=1;A1=1;A0=1;
}
#endif

/*
@name  JXC51_task_A_step
@param input
       flag - FLAG_RESET - reset the task to start a new step progress
            - FLAG_START - start step progress (PS: provide step number at the same time)
            - others     - advance progress until it ends
 */
static unsigned char JXC51_task_A_step(unsigned char input, uint8_t flag)
{
    static enum { step_null = 0,
                  step_ready,
                  step_start,
                  step_DRIVEon,
                  step_checkBUSY,
                  step_waitBUSY,
                  step_end } step_FSM_state;

    unsigned char err = 0;
    if (flag == FLAG_RESET)
    {
        step_FSM_state = step_ready;
    }
    else
    {
    }
    if (input != STEP_REQ_NULL)
    {
        JXC51_A_IN0 = (input & 0x01);
        input >>= 1;
        JXC51_A_IN1 = (input & 0x01);
        input >>= 1;
        JXC51_A_IN2 = (input & 0x01);
        input >>= 1;
#ifdef JXC51_A_IN3
        JXC51_A_IN3 = (input & 0x01);
        input >>= 1;
#endif
#ifdef JXC51_A_IN4
        JXC51_A_IN4 = (input & 0x01);
#endif
        //        input >>= 1;
        //        JXC51_A_IN4 = (input & 0x01);
        //        input >>= 1;
        //        JXC51_A_IN5 = (input & 0x01);
    }

    switch (step_FSM_state)
    {
    case step_null:
        break;
    case step_ready:
        if (flag == FLAG_START)
        {
            step_FSM_state = step_start;
        }
        else
        {
        }
        break;
    case step_start:
        JXC51_A_SVON = 1; // SVON = 1;
        step_FSM_state = step_DRIVEon;
        break;
    case step_DRIVEon:
        if (!JXC51_A_SVRE)
        {
        }
        else
        {
            JXC51_A_DRIVE = 1; // DRIVE = 1;
            step_FSM_state = step_checkBUSY;
        }
        break;
    case step_checkBUSY:
        if (!JXC51_A_BUSY)
        {
        } // wait until BUSY = 0;
        else
        {
            step_FSM_state = step_waitBUSY;
        }
        JXC51_A_DRIVE = 0; // DRIVE = 0;
        break;
    case step_waitBUSY:
        if ((!JXC51_A_BUSY) == 0)
        {
        } // wait until BUSY = 1;
        else
        {
            //                output = PORTB & 0x1F + JXC51_A_OUT5 * 0x20;
            step_FSM_state = step_end;
            //                JXC51_A_SVON = 0;
        }
        break;
    case step_end:
        step_FSM_state = step_ready;
        break;

    default:
        step_FSM_state = step_null;
        break;
    }
    return err;
}

#ifdef __USE_JXC51_B__
/*
@name  JXC51_task_B_step
@param input
       flag - FLAG_RESET - reset the task to start a new step progress
            - FLAG_START - start step progress (PS: provide step number at the same time)
            - others     - advance progress until it ends
 */
static unsigned char JXC51_task_B_step(unsigned char input, uint8_t flag)
{
    static enum { step_null = 0,
                  step_ready,
                  step_start,
                  step_DRIVEon,
                  step_checkBUSY,
                  step_waitBUSY,
                  step_end } step_FSM_state;

    unsigned char err = 0;
    if (flag == FLAG_RESET)
    {
        step_FSM_state = step_ready;
    }
    else
    {
    }
    if (input != STEP_REQ_NULL)
    {
        JXC51_B_IN0 = (input & 0x01);
        input >>= 1;
        JXC51_B_IN1 = (input & 0x01);
        input >>= 1;
        JXC51_B_IN2 = (input & 0x01);
        input >>= 1;
#ifdef JXC51_B_IN3
        JXC51_B_IN3 = (input & 0x01);
        input >>= 1;
#endif
#ifdef JXC51_B_IN4
        JXC51_B_IN4 = (input & 0x01);
#endif
        // input >>= 1;
        // JXC51_B_IN4 = (input & 0x01);
        // input >>= 1;
        // JXC51_B_IN5 = (input & 0x01);
    }

    switch (step_FSM_state)
    {
    case step_null:
        break;
    case step_ready:
        if (flag == FLAG_START)
        {
            step_FSM_state = step_start;
        }
        else
        {
        }
        break;
    case step_start:
        JXC51_B_SVON = 1; // SVON = 1;
        step_FSM_state = step_DRIVEon;
        break;
    case step_DRIVEon:
        if (!JXC51_B_SVRE)
        {
        }
        else
        {
            JXC51_B_DRIVE = 1; // DRIVE = 1;
            step_FSM_state = step_checkBUSY;
        }
        break;
    case step_checkBUSY:
        if (!JXC51_B_BUSY)
        {
        } // wait until BUSY = 0;
        else
        {
            step_FSM_state = step_waitBUSY;
        }
        JXC51_B_DRIVE = 0; // DRIVE = 0;
        break;
    case step_waitBUSY:
        if (!JXC51_B_BUSY == 0)
        {
        } // wait until BUSY = 1;
        else
        {
            // output = PORTB & 0x1F + JXC51_B_OUT5 * 0x20;
            step_FSM_state = step_end;
            // JXC51_B_SVON = 0;
        }
        break;
    case step_end:
        step_FSM_state = step_ready;
        break;

    default:
        step_FSM_state = step_null;
        break;
    }
    return err;
}
#endif

/*
@name  JXC51_task_B_setup
@param flag - FLAG_RESET - start a new setup progress
            - others     - advance progress until it ends
 */
#ifdef __USE_JXC51_B__
static unsigned char JXC51_task_B_setup(unsigned char flag)
{
    // static volatile unsigned char setup_A_flag = 0;
    static enum { setup_null = 0,
                  setup_reset,
                  setup_checkALM,
                  setup_SVON,
                  setup_start,
                  setup_SETUPcheck,
                  setup_SETONcheck,
                  setup_end } setup_FSM_state;

    if (flag == FLAG_RESET)
    {
        setup_FSM_state = setup_reset;
        JXC51_task_B_step(STEP_REQ_NULL, FLAG_RESET);
    }

    switch (setup_FSM_state)
    {
    case setup_reset:
        JXC51_B_SVON = 0;  // SVON = 0
        JXC51_B_SETUP = 0; // SETUP = 0
        setup_FSM_state = setup_checkALM;
        break;
    case setup_checkALM:
        if (JXC51_B_BUSY || !JXC51_B_ALARM) // if (BUSY||ALARM) && not RESET
        {
            JXC51_B_RESET = 1; // RESET = 1;
        }
        else
        {
            JXC51_B_RESET = 0; // RESET = 0
            setup_FSM_state = setup_SVON;
        }
        break;
    case setup_SVON:
        if (!JXC51_B_SVRE == 0) // SVRE = 0;
        {
        }
        else
        {
            JXC51_B_SVON = 1; // SVON = 1;
            setup_FSM_state = setup_start;
        }
        break;
    case setup_start:
        if (!JXC51_B_SVRE) // wait until SVRE = 1;
        {
        }
        else
        {
            JXC51_B_SETUP = 1; // SETUP = 1;
            setup_FSM_state = setup_SETUPcheck;
        }
        break;
    case setup_SETUPcheck:
        if (!JXC51_B_BUSY || (JXC51_B_SETON && !JXC51_B_INP))
        {
            JXC51_B_SETUP = 0; // SETUP = 0;
            setup_FSM_state = setup_SETONcheck;
        }
        break;
    case setup_SETONcheck:
        if (!JXC51_B_BUSY == 0 || JXC51_B_SETON || !JXC51_B_INP) // wait until BUSY=0 & SETON=1 & INP=1;
        {
            // setup_FSM_state = 0;
            setup_FSM_state = setup_end;
        }
        break;
    case setup_end:
        break;
    default:
        break;
    }
    return setup_FSM_state;
}
#endif

/*
@name  JXC51_task_AB_setup
@param flag - FLAG_RESET - start a new setup progress
            - others     - advance progress until it ends

 */
static unsigned char JXC51_task_AB_setup(unsigned char flag)
{
    static enum { setup_null = 0,
                  setup_reset,
                  setup_checkALM,
                  setup_SVON,
                  setup_start,
                  setup_SETUPcheck,
                  setup_SETONcheck,
                  setup_end } setup_FSM_state;

    if (flag == FLAG_RESET)
    {
        setup_FSM_state = setup_reset;
        JXC51_task_A_step(STEP_REQ_NULL, FLAG_RESET);
    }

    switch (setup_FSM_state)
    {
    case setup_reset:
        JXC51_A_SVON = 0;  // SVON = 0
        JXC51_A_SETUP = 0; // SETUP = 0
        setup_FSM_state = setup_checkALM;
        break;
    case setup_checkALM:
        if (JXC51_A_BUSY || !JXC51_A_ALARM) // if (BUSY||ALARM) && not RESET
        {
            JXC51_A_RESET = 1; // RESET = 1;
        }
        else
        {
            JXC51_A_RESET = 0; // RESET = 0
            setup_FSM_state = setup_SVON;
        }
        break;
    case setup_SVON:
        if (JXC51_A_SVRE) // SVRE = 0;
        {
        }
        else
        {
            JXC51_A_SVON = 1; // SVON = 1;
            setup_FSM_state = setup_start;
        }
        break;
    case setup_start:
        if (!JXC51_A_SVRE) // wait until SVRE = 1;
        {
        }
        else
        {
            JXC51_A_SETUP = 1; // SETUP = 1;
            setup_FSM_state = setup_SETUPcheck;
        }
        break;
    case setup_SETUPcheck:
        if (!JXC51_A_BUSY || (JXC51_A_SETON && !JXC51_A_INP))
        {
            JXC51_A_SETUP = 0; // SETUP = 0;
            setup_FSM_state = setup_SETONcheck;
        }
        break;
    case setup_SETONcheck:
        if (JXC51_A_BUSY || JXC51_A_SETON || (!JXC51_A_INP)) // wait until BUSY=0 & SETON=1 & INP=1;
        {
            // setup_FSM_state = 0;
#ifdef __USE_JXC51_B__
            JXC51_task_B_setup(FLAG_RESET);
#endif
            setup_FSM_state = setup_end;
        }
        break;
    case setup_end:
        break;
    default:
        break;
    }
    return setup_FSM_state;
}

void taskJXC51(void)
{
#ifdef __USE_IO_EXPAND__
    JXC51_A_READ();
#endif

    // advance the setup and step progress (if have)
    JXC51_task_AB_setup(FLAG_RUN);
    JXC51_task_A_step(STEP_REQ_NULL, FLAG_RUN);
#ifdef __USE_JXC51_B__
    JXC51_task_B_setup(FLAG_RUN);
    JXC51_task_B_step(STEP_REQ_NULL, FLAG_RUN);
#endif

#ifdef __USE_IO_EXPAND__
    JXC51_A_APPLY();
#endif
}

unsigned char jxc51_init(void)
{
    jxc51_A_step_finish_flag = 0;
#ifdef __USE_JXC51_B__
    jxc51_B_step_finish_flag = 0;
#endif
    jxc51_setup_finish_flag = 0;
    JXC51_task_AB_setup(FLAG_RESET);
    jxc51_A_goto(STEP_REQ_NULL);
#ifdef __USE_JXC51_B__
    jxc51_B_goto(STEP_REQ_NULL);
#endif
    return 0;
}

unsigned char jxc51_return(void)
{
    jxc51_setup_finish_flag = 0;
    JXC51_task_AB_setup(FLAG_RESET);
    jxc51_A_goto(STEP_REQ_NULL);
#ifdef __USE_JXC51_B__
    JXC51_task_B_setup(FLAG_RUN);
    jxc51_B_goto(STEP_REQ_NULL);
#endif
    return 0;
}

unsigned char jxc51_A_holdRelease(void)
{
    unsigned char output = 0;
    if (!JXC51_A_HOLD && JXC51_A_BUSY) // if(HOLD = 0 && BUSY == 1)
    {
        JXC51_A_HOLD = 1;
    }
    else if (!JXC51_A_BUSY && JXC51_A_HOLD)
    {
        JXC51_A_HOLD = 0;
    }
    else
    {
        return 0x80;
    }
    return output;
}

unsigned char jxc51_A_goto(uint8_t step_no)
{
    if (step_no == STEP_REQ_NULL)
    {
        jxc51_A_stored_step = 0;
        return 0;
    }

    if (step_no != jxc51_A_stored_step && !JXC51_A_BUSY)
    {
        jxc51_A_step_finish_flag = 0;
        jxc51_A_stored_step = step_no;
        JXC51_task_A_step(step_no, FLAG_START);
        // what to do if error occur when step?
        // what if give a 0 right after reset?
    }
    else
    {
        // input step_no invalid, do nothing.
    }
    return 0;
}

/*
@name  jxc51_A_position
@brief renew jxc51_position and return
 */
unsigned char jxc51_A_position(void)
{
    static uint8_t jxc51_position;
    jxc51_position = (JXC51_A_OUT2 * 4) + (JXC51_A_OUT1 * 2) + JXC51_A_OUT0;

    return jxc51_position;
}

unsigned char jxc51_A_checkSTEP(void)
{
    // step over
    if (jxc51_A_step_finish_flag == 0)
    {
        if (JXC51_A_INP && (jxc51_A_position() == jxc51_A_stored_step))
        {
            jxc51_A_step_finish_flag = 1;
        }
        else
        {
        }
    }
    else
    {
    }

    return jxc51_A_step_finish_flag;
}

unsigned char jxc51_A_checkSETUP(void)
{
    // setup over
    if (jxc51_setup_finish_flag == 0)
    {
        if (JXC51_A_INP && (jxc51_A_position() == 0))
        {
            jxc51_setup_finish_flag = 1;
        }
        else
        {
        }
    }
    else
    {
    }

    return jxc51_setup_finish_flag;
}

#ifdef __USE_JXC51_B__
unsigned char jxc51_B_holdRelease(void)
{
    unsigned char output = 0;
    if (JXC51_B_HOLD == 0 && !JXC51_B_BUSY == 0) // if(HOLD = 0 && BUSY == 1)
    {
        JXC51_B_HOLD = 1;
    }
    else if (!JXC51_B_BUSY && JXC51_B_HOLD)
    {
        JXC51_B_HOLD = 0;
    }
    else
    {
        return 0x80;
    }
    return output;
}

unsigned char jxc51_B_goto(uint8_t step_no)
{
    if (step_no == STEP_REQ_NULL)
    {
        jxc51_B_stored_step = 0;
        return 0;
    }

    if (step_no != jxc51_B_stored_step && !JXC51_B_BUSY)
    {
        jxc51_B_step_finish_flag = 0;
        jxc51_B_stored_step = step_no;
        JXC51_task_B_step(step_no, FLAG_START);
        // what to do if error occur when step?
        // what if give a 0 right after reset?
    }
    else
    {
        // input step_no invalid, do nothing.
    }
    return 0;
}

/*
@name  jxc51_B_position
@brief renew jxc51_position and return
 */
unsigned char jxc51_B_position(void)
{
    static uint8_t jxc51_position;
    jxc51_position = (JXC51_B_OUT2 * 4) + (JXC51_B_OUT1 * 2) + JXC51_B_OUT0;

    return jxc51_position;
}

unsigned char jxc51_B_checkSTEP(void)
{
    // step over
    if (jxc51_B_step_finish_flag == 0)
    {
        if (JXC51_B_INP && (jxc51_B_position() == jxc51_B_stored_step))
        {
            jxc51_B_step_finish_flag = 1;
        }
    }

    return jxc51_B_step_finish_flag;
}

unsigned char jxc51_B_checkSETUP(void)
{
    // setup over
    if (jxc51_setup_finish_flag == 0)
    {
        if (JXC51_B_INP && (jxc51_B_position() == 0))
        {
            jxc51_setup_finish_flag = 1;
        }
    }

    return jxc51_setup_finish_flag;
}
#endif
