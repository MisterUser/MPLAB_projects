/******************************************************************************/
/* User Level #define Macros                                                  */
/******************************************************************************/

//LED
#define MSLED_TRIS		(TRISBbits.TRISB15)		// LED on MicroStick
#define MSLED_I			(PORTBbits.RB15)
#define MSLED_O			(LATBbits.LATB15)

//OUTPUT 1 - Bit1 is LSB
#define O1_Bit4         (LATAbits.LATA0)
#define O1_Bit3         (LATAbits.LATA1)
#define O1_Bit2         (LATAbits.LATA2)
#define O1_Bit1         (LATAbits.LATA4)





        
/******************************************************************************/
/* User Function Prototypes                                                   */
/******************************************************************************/

void InitApp(void);         /* I/O and Peripheral Initialization */

void update_O1();