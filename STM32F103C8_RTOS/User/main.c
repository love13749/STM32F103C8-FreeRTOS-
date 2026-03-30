#include "stm32f10x.h" // Device header
#include "FreeRTOS.h"  //FreeRTOS操作系统
#include "task.h"      //FreeRTOS任务管理
static TaskHandle_t APP_Handle;  // APP 任务句柄
static TaskHandle_t Task1_Handle;// 任务1句柄

BaseType_t xstatus = 0;// 任务创建状态
void BSP_Init(void);   // 硬件初始化函数声明
void Task_CreatAPP(void);// APP 任务创建函数声明
void Task1(void *p);// 任务1函数声明

int main(void)
{
	// 硬件初始化（使能时钟、配置 GPIO）
  BSP_Init();

  // 创建任务1：点亮 LED
  xstatus = xTaskCreate(Task1,          // 任务函数
                        "Task1",        // 任务名称
                        128,            // 任务堆栈大小（单位字，根据实际调整）
                        NULL,           // 任务参数
                        1,              // 任务优先级（1 为最低，数值越大优先级越高）
                        &Task1_Handle); // 任务句柄

  // 如果任务创建失败，可在此处添加错误处理（例如死循环）
  if (xstatus != pdPASS)
  {
      while (1);
  }

  // 启动调度器，任务开始运行
  vTaskStartScheduler();

  //删除任务
  vTaskDelete(APP_Handle);

  // 正常情况下不会执行到这里
  while (1);
}

/**
 * @brief 硬件初始化：配置 PA1 为推挽输出，并使能时钟
 */
void BSP_Init(void)
{
    // 使能 GPIOA 时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

    // 配置 PA1 为推挽输出，最大速度 50MHz
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
}

/**
 * @brief 任务1：点亮 LED（PA1 低电平）
 * @param p 任务参数（未使用）
 */
void Task1(void *p)
{
    // 点亮 LED（PA1 输出低电平）
    GPIO_ResetBits(GPIOA, GPIO_Pin_1);

    // 任务保持运行，避免空循环占用 CPU，可延时或删除自身
    for (;;)
    {
        vTaskDelay(pdMS_TO_TICKS(1000)); // 延时 1 秒（实际任务不执行任何操作，仅保持存在）
    }
}

/**
 * @brief 创建 APP 任务（未使用，可根据需要实现）
 */
void Task_CreatAPP()
{
    // 此处可根据实际需求创建 APP 任务，示例中暂不实现
}
