#include "cgi_process.h"

#include <stdio.h>
#include <string.h>

#include "button.h"
#include "lwip/apps/httpd.h"

char const *ssi_tags[] = {
    "TIME", "SENSOR", "BUTTON",
    "RLED", "BLED", "GLED", "LED4",
    "RBULB", "BBULB", "GBULB", "CBULB"
};
char const **tags = ssi_tags;

extern ADC_HandleTypeDef hadc1;

static const char *led_cgi_handler(int iIndex, int iNumParams,
                                   char *pcParam[], char *pcValue[]);

const tCGI LED_CGI = {"/leds.cgi", led_cgi_handler};
tCGI CGI_ARR[NUM_OF_CGIS];

static uint16_t insert_text(char *destination, int capacity, const char *text)
{
    if((destination == NULL) || (text == NULL) || (capacity <= 0))
    {
        return 0;
    }

    int written = snprintf(destination, (size_t)capacity, "%s", text);
    if(written < 0)
    {
        return 0;
    }

    if(written >= capacity)
    {
        return (uint16_t)(capacity - 1);
    }

    return (uint16_t)written;
}

static uint16_t insert_number(char *destination, int capacity, uint32_t value)
{
    if((destination == NULL) || (capacity <= 0))
    {
        return 0;
    }

    int written = snprintf(destination, (size_t)capacity, "%lu",
                           (unsigned long)value);
    if(written < 0)
    {
        return 0;
    }

    if(written >= capacity)
    {
        return (uint16_t)(capacity - 1);
    }

    return (uint16_t)written;
}

static const char *led_state_class(uint32_t led)
{
    return led_is_on(led) ? "is-on" : "is-off";
}

static const char *lamp_image(uint32_t led, const char *on_image)
{
    return led_is_on(led) ? on_image : "/lightoff.png";
}

uint16_t ssi_handler(int iIndex, char *pcInsert, int iInsertLen)
{
    switch(iIndex)
    {
        case 0:
            return insert_number(pcInsert, iInsertLen, HAL_GetTick());
        case 1:
            return insert_number(pcInsert, iInsertLen,
                                 HAL_ADC_GetValue(&hadc1));
        case 2:
            return insert_number(pcInsert, iInsertLen, get_btn_state());
        case 3:
            return insert_text(pcInsert, iInsertLen,
                               led_state_class(RED_LED));
        case 4:
            return insert_text(pcInsert, iInsertLen,
                               led_state_class(BLUE_LED));
        case 5:
            return insert_text(pcInsert, iInsertLen,
                               led_state_class(GREEN_LED));
        case 6:
            return insert_text(pcInsert, iInsertLen,
                               led_state_class(CUSTOM_LED));
        case 7:
            return insert_text(pcInsert, iInsertLen,
                               lamp_image(RED_LED, "/redon.png"));
        case 8:
            return insert_text(pcInsert, iInsertLen,
                               lamp_image(BLUE_LED, "/blueon.png"));
        case 9:
            return insert_text(pcInsert, iInsertLen,
                               lamp_image(GREEN_LED, "/greenon.png"));
        case 10:
            return insert_text(pcInsert, iInsertLen,
                               lamp_image(CUSTOM_LED, "/yellowon.png"));
        default:
            return 0;
    }
}

static uint32_t led_from_parameter(const char *value)
{
    if(strcmp(value, "1") == 0)
    {
        return RED_LED;
    }
    if(strcmp(value, "2") == 0)
    {
        return BLUE_LED;
    }
    if(strcmp(value, "3") == 0)
    {
        return GREEN_LED;
    }
    if(strcmp(value, "4") == 0)
    {
        return CUSTOM_LED;
    }

    return 0U;
}

static const char *led_name(uint32_t led)
{
    switch(led)
    {
        case RED_LED:
            return "Red";
        case BLUE_LED:
            return "Blue";
        case GREEN_LED:
            return "Green";
        case CUSTOM_LED:
            return "Yellow";
        default:
            return "Unknown";
    }
}

static const char *led_cgi_handler(int iIndex, int iNumParams,
                                   char *pcParam[], char *pcValue[])
{
    if(iIndex != 0)
    {
        return "/led_control.shtml";
    }

    for(int i = 0; i < iNumParams; i++)
    {
        if(strcmp(pcParam[i], "led") != 0)
        {
            continue;
        }

        uint32_t led = led_from_parameter(pcValue[i]);
        if(led == 0U)
        {
            continue;
        }

        led_toggle(led);
        printf("%s LED is now %s.\r\n", led_name(led),
               led_is_on(led) ? "ON" : "OFF");
        break;
    }

    return "/led_control.shtml";
}
