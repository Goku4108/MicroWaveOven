#include <xc.h>
#include "clcd.h"
#include "main.h"
#include "microoven.h"
#include "matrix_keypad.h"
#include "timers.h"

#pragma config WDTE = OFF

unsigned char operational_flag = POWER_ON_SCREEN ;
unsigned char reset_flag = RESET_NOTHING;
extern unsigned char sec, min;
 
 

void init_config(void) {
    
    init_clcd(); //config clcd
    init_matrix_keypad(); // config keypad
    
    FAN_DDR = 0; //config FAN
    FAN = 0; // initial status of FAN
    
    BUZZER_DDR = 0;// config buzzer
    BUZZER = 0; //initial status of buzzer
    
    init_timer2();
    PEIE = 1;
    GIE = 1;
    
}

void main(void) {
    
    unsigned char key;
    unsigned char start_mode = 0;
    init_config();
    while(1) {
        
        key = read_matrix_keypad(STATE);
        // select the menu based on keypress (operational flag)
        if(operational_flag == MENU_SCREEN) {
             
        if (key == 1) {
            clear_screen();
            operational_flag = MICRO_MODE;
            start_mode = 0;
            clcd_print("Power = 900W", LINE2(0));
            __delay_ms(2000);
            clear_screen();
            reset_flag = RESET_MODE; 
        }
        else if (key == 2) {
            
            clear_screen();
            start_mode = 0;
            operational_flag = GRILL_MODE;
            reset_flag = RESET_MODE;
            
        }
        else if (key == 3) {
            
            clear_screen();
            start_mode = 0;
            operational_flag = CONVENTION_MODE;
            reset_flag = RESET_MODE;
        }
        else if (key == 4) {
            
            clear_screen();
            operational_flag = START_MODE;
            start_mode = 1;
            reset_flag = RESET_MODE;
        }
      }
      if(operational_flag == DISPLAY_TIME) {
          
          if (key == 4) {
             //operational_flag = START; (not using anywhere)
              // Increment the cooking time by 30 sec
              if(start_mode) {
                  
                  sec = sec+30;
                  if(sec > 59) {
                      
                      min++;
                      sec = sec - 60;
                  }
              }
                  
        }
        else if (key == 5) {
            
            operational_flag = PAUSE;
          
        }
        else if (key == 6) {
            
            operational_flag = STOP;
        }
        
          
      } 
      else if (operational_flag == PAUSE) {
          if (key == 4){ // resume the oven
              
              FAN = 1;
              TMR2ON = 1;
              operational_flag = DISPLAY_TIME;
              
          }
      } 
        
        
        
        
        
        
        
        switch(operational_flag) {
            
            case POWER_ON_SCREEN :
                power_on_screen();
           
                operational_flag = MENU_SCREEN;
                clear_screen(); //to clear the clcd
            break;
            case MENU_SCREEN :
                                
                display_menu_screen();
                break;
            case MICRO_MODE :
                set_time(key, reset_flag);
                
                break;
            case DISPLAY_TIME :
                time_display();
                break;
            
            case PAUSE :
                FAN = 0;
                TMR2ON = 0;
                break;
                
            case STOP : 
                FAN = 0;
                TMR2ON = 0;
                clear_screen();
                operational_flag = MENU_SCREEN;
                break;
                
            case GRILL_MODE :
                set_time(key, reset_flag);
                            
                break;
            case CONVENTION_MODE :
                set_temp(key, reset_flag);

                break;
            case START_MODE :
                //display time as 30sec for cooking time
                heat_food(); 
                break;
                                
             
           
        } // Switch
        reset_flag = RESET_NOTHING; 
    } // while (1)
    
} // main
