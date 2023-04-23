/*##############################################################################
Prof. Paulo Denis Garcez da Luz - 2022/1S
Desenvolvido para a placa EK-TM4C1294XL utilizando o SDK TivaWare no IAR
Programa exemplo de blink usando 4 led, 2 bot�es e Perif�rico SysTick.
##############################################################################*/
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "driverlib/debug.h"
#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"
#include "driverlib/systick.h"

// vari�vel que conta os ticks(1ms) - Volatile n�o permite o compilador otimizar o c�digo
volatile unsigned int SysTicks1ms;

// Prot�tipos de fun��es criadas no programa, c�digo depois do main
void SysTickIntHandler(void);
void SetupSystick(void);

int main(void)
{
  // LAB2 CODE
  bool gameRunnning = false;
  bool gameStarted = false;
  unsigned int gameStartTime = 1000;
  unsigned int gameEndTime = gameStartTime + 3000;
  unsigned int totalTime = 0;
  // END LAB2 CODE

  // estado de cada led
  bool Led1 = false, Led4 = false;
  // controle via programa��o de anti-debouncing
  bool bt1flag = false, bt2flag = false;
  // controle de tempo para cada bot�o
  unsigned int bt1time = 0, bt2time = 0;
  // Configura o clock para utilizar o xtal externo de 25MHz
  SysCtlClockFreqSet(SYSCTL_XTAL_25MHZ | SYSCTL_OSC_MAIN | SYSCTL_USE_OSC, 25000000);
  // executa configura��o e inicializa��o do SysTick
  SetupSystick();
  // habilitar gpio port N
  SysCtlPeripheralEnable(SYSCTL_PERIPH_GPION);
  // aguardar o perif�rico ficar pronto para uso
  while (!SysCtlPeripheralReady(SYSCTL_PERIPH_GPION))
  { /*Espera habilitar o port*/
  }
  // habilitar gpio port F
  SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
  // aguardar o perif�rico ficar pronto para uso
  while (!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOF))
  { /*Espera habilitar o port*/
  }
  // configura o pin_0 e pin_1 como sa�da
  // habilitar gpio port J
  SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOJ);
  // aguardar o perif�rico ficar pronto para uso
  while (!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOJ))
  { /*Espera habilitar o port*/
  }
  GPIOPinTypeGPIOOutput(GPIO_PORTN_BASE, GPIO_PIN_0 | GPIO_PIN_1);
  // configura o pin_0 e pin_4 como sa�da
  GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_0 | GPIO_PIN_4);
  // configura o pin_0 e pin_1 como entrada
  GPIOPinTypeGPIOInput(GPIO_PORTJ_BASE, GPIO_PIN_0 | GPIO_PIN_1);
  // configura os pinos para 2mA como limite de corrente e com week pull-up
  GPIOPadConfigSet(GPIO_PORTJ_BASE, GPIO_PIN_0 | GPIO_PIN_1, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);

  // loop infinito
  while (1)
  {
    //debounce botão 1
    if (bt1flag)
    {
      // Bot�o1 liberado !!!
      if (GPIOPinRead(GPIO_PORTJ_BASE, GPIO_PIN_0) && SysTicks1ms >= bt1time)
      {
        // bot�o liberado
        bt1flag = false;
        // 55ms para liberar estado do bot�o ... tempo anti-debouncing
        bt1time = SysTicks1ms + 55;
      }
    }
    else
    {
      // bot�o1 pressionado !!!
      if ((GPIOPinRead(GPIO_PORTJ_BASE, GPIO_PIN_0) == 0) && (SysTicks1ms >= bt1time))
      {
        // bot�o pressionado
        bt1flag = true;
        // 55ms para liberar estado do bot�o ... tempo anti-debouncing
        bt1time = SysTicks1ms + 55;
        // altera o estado do led ligado / desligado
        Led1 = false;
        // escreve o estado do led no pino
        GPIOPinWrite(GPIO_PORTN_BASE, GPIO_PIN_1, Led1 << 1);
        gameRunnning = false;
        gameStarted = true;
        Led4 = false;
        // escreve o estado do led no pino
        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_0, Led4);
        printf("\nJOGO INICIADO \n");
        
        gameStartTime = SysTicks1ms + 1000;
      }
      //Dá start no game caso o botão 1 já tenha sido pressionado
      else if (gameStarted)
      {
        if (!gameRunnning)
        {
          if (SysTicks1ms >= gameStartTime)
          {
            gameRunnning = true;
            Led1 = true;
            GPIOPinWrite(GPIO_PORTN_BASE, GPIO_PIN_1, Led1 << 1);
            gameStartTime = SysTicks1ms;
            gameEndTime = gameStartTime + 3000;
          }
        }
        else
        {
          if (SysTicks1ms >= gameEndTime)
          {
            printf("GAME OVER\n");
            // altera o estado do led ligado / desligado
            Led1 = false;
            // escreve o estado do led no pino
            GPIOPinWrite(GPIO_PORTN_BASE, GPIO_PIN_1, Led1 << 1);
            Led4 = true;
            // escreve o estado do led no pino
            GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_0, Led4);
            gameRunnning = false;
            gameStarted = false;
          }
          //debounce botão 2
          else if (bt2flag) 
          {
            // bot�o2 liberado !!!
            if (GPIOPinRead(GPIO_PORTJ_BASE, GPIO_PIN_1) && (SysTicks1ms >= bt2time))
            {
              // bot�o liberado
              bt2flag = false;
              // 55ms para liberar estado do bot�o ... tempo anti-debouncing
              bt2time = SysTicks1ms + 55;
            }
          }
          else
          {
            // bot�o2 pressionado !!!
            if ((GPIOPinRead(GPIO_PORTJ_BASE, GPIO_PIN_1) == 0) && (SysTicks1ms >= bt2time))
            {
              // bot�o pressionado
              bt2flag = true;
              // 55ms para liberar estado do bot�o ... tempo anti-debouncing
              bt2time = SysTicks1ms + 55;
              // altera o estado do led ligado / desligado
              Led1 = false;
              // escreve o estado do led no pino
              GPIOPinWrite(GPIO_PORTN_BASE, GPIO_PIN_1, Led1 << 1);
              totalTime = SysTicks1ms - gameStartTime;
              gameRunnning = false;
              gameStarted = false;

              printf("Tempo total: %dms\n", totalTime);
            }
          }
        }
      }
    }
  }
}

// fun��o de tratamento da interrup��o do SysTick
void SysTickIntHandler(void)
{
  SysTicks1ms++;
}

// fun��o para configurar e inicializar o perif�rico Systick a 1ms
void SetupSystick(void)
{
  SysTicks1ms = 0;
  // desliga o SysTick para poder configurar
  SysTickDisable();
  // clock 25MHz <=> SysTick deve contar 1ms=25k-1 do Systick_Counter - 12 trocas de contexto PP->IRQ - (1T Mov, 1T Movt, 3T LDR, 1T INC ... STR e IRQ->PP j� n�o contabilizam atrasos para a vari�vel)
  SysTickPeriodSet(25000 - 1 - 12 - 6);
  // registra a fun��o de atendimento da interrup��o
  SysTickIntRegister(SysTickIntHandler);
  // liga o atendimento via interrup��o
  SysTickIntEnable();
  // liga novamente o SysTick
  SysTickEnable();
}