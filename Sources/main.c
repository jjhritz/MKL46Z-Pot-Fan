#include "global.h"
#include "port.h"
#include "int.h"
#include "pit.h"
#include "adc.h"
#include "pot.h"
#include "tpm.h"
#include "motor.h"
#include "tpm_pwm.h"

#define forever for(;;)

static bool_t g_read_pot;


tpm_timer_t g_motor_tpm_timer = tpm_timer_0;
tpm_timer_t g_tpm0_ps = tpm_ps_4;
tpm_channel_t g_motor_tpm_chan = tpm_channel_0;
pwm_polarity_t g_motor_pwm_polarity = pwm_polarity_high;
tpm_timer_t g_busy_tpm_timer = tpm_timer_1;

void pit_callback()
{
	g_read_pot = true;
}

void hardware_init()
{
	//mask all interrupts
	int_mask_all();
	
	//Enable SIM clock to ports C
	SIM_SCGC5 |= 1 << 11;
	
	//Enable SIM clock to PIT, TPM0, TPM1, & ADC0
	SIM_SCGC6 &= (0b00000 << 23);
	SIM_SCGC6 |= (0b10111 << 23);
	
	//configure pin PTC1 for TPM0_CH0 PWM
	PORT_PCR(PORT_C, 1) = 0;
	PORT_PCR(PORT_C, 1) |= (0b100 << 8);
	
	//configure pin PTC2 for ADC
	PORT_PCR(PORT_C, 2) = 0;
	PORT_PCR(PORT_C, 2) &= (0b000 << 8);
	
	//Initialize PIT0
	pit_init(pit_timer_0, pit_freq_10_hz, pit_state_disabled, pit_callback);
	
    // Initialize ADC.
    adc_init();

    // Initialize the POT module.
    pot_init();
    
    //Initialize TPM0_CH0 for PWM
    /*
    tpm_init_pwm(g_motor_tpm_timer, g_motor_tpm_chan, g_tpm0_ps, 
    				g_motor_pwm_polarity, 40);
    */
    
	SIM_SOPT2 |= SIM_SOPT2_TPMSRC(0b11);
	
	// Initialize TPM0_SC. We: (1) disable the TPM0 counter by writing 0b00 to TPM0_SC[CMOD] (we will write 0b01 later
	// to enable the counter to begin up counting); (2) DMA is disabled; (3) timer overflow interrupts are disabled;
	// (4) the TPM counter will be an up counter; (5) the prescale divider value is 2^2; this makes the TPM clock
	// period 1 us.

    TPM0_SC = 0x00000002;
    TPM0_CONF = 0;
    TPM0_CNT = 0;
    TPM0_MOD = 40 - 1;
    TPM0_C0SC = 0b00101000;
    
    //Initialize TPM 1
    //tpm_init(g_busy_tpm_timer, g_tpm0_ps, tpm_state_disabled);
    
    TPM1_SC = 2;
	
	//Unmask all interrupts
	int_unmask_all();
	
    //Enable PIT0 to begin generating interrupts.
    pit_cfg_state(pit_timer_0, pit_state_enabled);
    
    //Enable TPMs
    //tpm_cfg_state(g_motor_tpm_timer, tpm_state_enabled, g_tpm0_ps);
    //tpm_cfg_state(g_busy_tpm_timer, tpm_state_enabled, g_tpm0_ps);
    TPM0_SC |= (0b01 << 3);
    TPM1_SC |= (0b01 << 3);
}

void sw_init()
{
	g_read_pot = false;
}

void run()
{
    // We have nothing else to do in this particular application, but if we did, we would do it here.
    forever 
    {
    	//POT reads from 1-1700
    	if(g_read_pot == true)
    	{
        	uint16_t curr_pot = pot_get_curr_state();
        	
        	if(curr_pot < 700)
        	{
        		motor_cfg_speed(motor_speed_0);
        		g_read_pot = false;
        	}
        	else if(curr_pot >= 700 && curr_pot < 1200)
        	{
        		motor_cfg_speed(motor_speed_50);
        		g_read_pot = false;
        	}
        	else
        	{
        		motor_cfg_speed(motor_speed_100);
        		g_read_pot = false;
        	}
    	}

    }
}

int main()
{
    hardware_init();
    sw_init();
    run();
    return 0;
}
