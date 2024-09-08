// #define PIC1_C 0x20
// #define PIC1_D 0x21
// #define PIC2_C 0xa0
// #define PIC2_D 0xa1

// #define ICW1_DEF 0x10
// #define ICW1_ICW4 0x01
// #define ICW4_x86 0x01

// void cls();
// void setMonitorColor(char);

// void printString(char*);
// void printChar(char);

// void scroll();

// void printColorString(char* , char);
// void printColorChar(char , char);

// void initIDT();
// extern  void loadIdt();
// extern  void isr1_Handler();
// void handleKeypress(int);
// void pressed(char);
// void picRemap();

// unsigned char inportb(unsigned short);
// void outportb(unsigned short , unsigned char);

// char* TM_START;
// int CELL;

// struct IDT_ENTRY{
//     unsigned short base_Lower;
//     unsigned short selector;
//     unsigned char zero;
//     unsigned char flags;
//     unsigned short base_Higher;
// };

// struct IDT_ENTRY idt[256];
// extern unsigned int isr1;
// unsigned int base;

// int start(){
// 	TM_START = (char*) 0xb8000;
// 	CELL = 0;
// 	base = (unsigned int)&isr1;

// 	cls();
// 	setMonitorColor(0xa5);

// 	char Welcome[] = "Welcome To OS0 : Copyright 2021\n";
// 	char Welcome2[] = "Command Line Version 1.0.0.0\n\n";
// 	char OSM[] = "OS0 > ";

// 	printString(Welcome);
// 	printString(Welcome2);
// 	printColorString(OSM , 0xa8);

// 	initIDT();
// }


// void cls(){
// 	int i = 0;
// 	CELL = 0;
// 	while(i < (2 * 80 * 25)){
// 		*(TM_START + i) = ' '; // Clear screen
// 		i += 2;
// 	}
// }

// void setMonitorColor(char Color){
// 	int i = 1;
// 	while(i < (2 * 80 * 25)){
// 		*(TM_START + i) = Color;
// 		i += 2;
// 	}
// }

// void printString(char* cA){
// 	int i = 0;
// 	while(*(cA + i) != '\0'){
// 		printChar(*(cA + i));
// 		i++;
// 	}
// }

// void printChar(char c){
// 	if(CELL == 2 * 80 * 25)
// 		scroll();
// 	if(c == '\n'){
// 		CELL = ((CELL + 160) - (CELL % 160));
// 		return;
// 	}
// 	*(TM_START + CELL) = c;
// 	CELL += 2;	
// }

// void scroll(){
// 	int i = 160 , y = 0;
// 	while(i < 2 * 80 * 25){
// 		*(TM_START + y) = *(TM_START + i);
// 		i += 2;
// 		y += 2;
// 	}
// 	CELL = 2 * 80 * 24;
// 	i = 0;
// 	while(i < 160){
// 		*(TM_START + CELL + i) = ' ';
// 		i += 2;
// 	}
// }

// void printColorString(char* c , char co){
// 	int i = 0;
// 	while(*(c + i) != '\0'){
// 		printColorChar(*(c + i) , co);
// 		i++;
// 	}
// }

// void printColorChar(char c , char co){
// 	if(CELL == 2 * 80 * 25)
// 		scroll();
// 	if(c == '\n'){
// 		CELL = ((CELL + 160) - (CELL % 160));
// 		return;
// 	}
// 	*(TM_START + CELL) = c;
// 	*(TM_START + CELL + 1) = co;
// 	CELL += 2;	
// }

// void initIDT(){
// 	idt[1].base_Lower = (base & 0xFFFF);
// 	idt[1].base_Higher = (base >> 16) & 0xFFFF;
// 	idt[1].selector = 0x08;
// 	idt[1].zero = 0;
// 	idt[1].flags = 0x8e;

// 	picRemap();

// 	outportb(0x21 , 0xfd);
// 	outportb(0xa1 , 0xff);

// 	loadIdt();
// }

// unsigned char inportb(unsigned short _port){
//     unsigned char rv;
//     __asm__ __volatile__ ("inb %1, %0" : "=a" (rv) : "dN" (_port));
//     return rv;
// }

// void outportb(unsigned short _port, unsigned char _data){
//     __asm__ __volatile__ ("outb %1, %0" : : "dN" (_port), "a" (_data));
// }

// extern void isr1_Handler(){
// 	handleKeypress(inportb(0x60));
// 	outportb(0x20 , 0x20);
// 	outportb(0xa0 , 0x20);
// }

// void handleKeypress(int code){
// 	char Scancode[] = {
// 		0 , 0 , '1' , '2' ,
// 		'3' , '4' , '5' , '6' , 
// 		'7' , '8' , '9' , '0' , 
// 		'-' , '=' , 0 , 0 , 'Q' , 
// 		'W' , 'E' , 'R' , 'T' , 'Y' ,
// 		'U' , 'I' , 'O' , 'P' , '[' , ']' , 
// 		0 , 0 , 'A' , 'S' , 'D' , 'F' , 'G' , 
// 		'H' , 'J' , 'K' , 'L' , ';' , '\'' , '`' , 
// 		0 , '\\' , 'Z' , 'X' , 'C' , 'V' , 'B' , 'N' , 'M' ,
// 		',' , '.' , '/' , 0 , '*' , 0 , ' '
// 	};
	
// 	if(code == 0x1c)
// 		printChar('\n');
// 	else if(code < 0x3a)
// 		pressed(Scancode[code]);
// }

// void pressed(char key){
// 	printChar(key);
// }

// void picRemap(){
// 	unsigned char a , b;
// 	a = inportb(PIC1_D);
// 	b = inportb(PIC2_D);

// 	outportb(PIC1_C , ICW1_DEF | ICW1_ICW4);
// 	outportb(PIC2_C , ICW1_DEF | ICW1_ICW4);

// 	outportb(PIC1_D , 0);
// 	outportb(PIC2_D , 8);

// 	outportb(PIC1_D , 4);
// 	outportb(PIC2_D , 2);

// 	outportb(PIC1_D , ICW4_x86);
// 	outportb(PIC2_D , ICW4_x86);

// 	outportb(PIC1_D , a);
// 	outportb(PIC2_D , b);
// }



#include <stdint.h>

#define PIC1_C 0x20  // PIC1 Command Port
#define PIC1_D 0x21  // PIC1 Data Port
#define PIC2_C 0xa0  // PIC2 Command Port
#define PIC2_D 0xa1  // PIC2 Data Port

#define ICW1_DEF 0x10
#define ICW1_ICW4 0x01
#define ICW4_x86 0x01

void cls();
void setMonitorColor(char color);

void printString(char* str);
void printChar(char c);

void scroll();

void printColorString(char* str, char color);
void printColorChar(char c, char color);

void initIDT();
extern void loadIdt();
extern void isr1_Handler();
void handleKeypress(int scancode);
void pressed(char key);
void picRemap();

unsigned char inportb(unsigned short port);
void outportb(unsigned short port, unsigned char data);

char* TM_START;  // Starting address for VGA text memory
int CELL;  // Keeps track of the current cell position

struct IDT_ENTRY {
    unsigned short base_Lower;
    unsigned short selector;
    unsigned char zero;
    unsigned char flags;
    unsigned short base_Higher;
};

struct IDT_ENTRY idt[256];
extern unsigned int isr1;
unsigned int base;

int start() {
    TM_START = (char*)0xb8000;  // VGA text mode memory
    CELL = 0;
    base = (unsigned int)&isr1;  // ISR1 address

    cls();  // Clear screen
    setMonitorColor(0xa5);  // Set monitor color

    // Display welcome messages
    char Welcome[] = "Welcome To OS0 : Copyright 2021\n";
    char Welcome2[] = "Command Line Version 1.0.0.0\n\n";
    char OSM[] = "OS0 > ";

    printString(Welcome);
    printString(Welcome2);
    printColorString(OSM, 0xa8);

    // Initialize the Interrupt Descriptor Table
    initIDT();
    
    return 0;
}

void cls() {
    int i = 0;
    CELL = 0;
    while (i < (2 * 80 * 25)) {
        *(TM_START + i) = ' ';  // Clear the screen
        i += 2;
    }
}

void setMonitorColor(char color) {
    int i = 1;
    while (i < (2 * 80 * 25)) {
        *(TM_START + i) = color;
        i += 2;
    }
}

void printString(char* str) {
    int i = 0;
    while (*(str + i) != '\0') {
        printChar(*(str + i));
        i++;
    }
}

void printChar(char c) {
    if (CELL == 2 * 80 * 25)
        scroll();
    
    if (c == '\n') {
        CELL = ((CELL + 160) - (CELL % 160));  // Move to the next line
        return;
    }

    *(TM_START + CELL) = c;  // Print the character to the screen
    CELL += 2;  // Move to the next cell (skip attribute byte)
}

void scroll() {
    int i = 160, y = 0;
    while (i < 2 * 80 * 25) {
        *(TM_START + y) = *(TM_START + i);  // Move each line up
        i += 2;
        y += 2;
    }
    CELL = 2 * 80 * 24;
    
    // Clear the last line
    i = 0;
    while (i < 160) {
        *(TM_START + CELL + i) = ' ';
        i += 2;
    }
}

void printColorString(char* str, char color) {
    int i = 0;
    while (*(str + i) != '\0') {
        printColorChar(*(str + i), color);
        i++;
    }
}

void printColorChar(char c, char color) {
    if (CELL == 2 * 80 * 25)
        scroll();
    
    if (c == '\n') {
        CELL = ((CELL + 160) - (CELL % 160));  // Move to the next line
        return;
    }

    *(TM_START + CELL) = c;  // Print the character to the screen
    *(TM_START + CELL + 1) = color;  // Set the character's color
    CELL += 2;  // Move to the next cell (skip attribute byte)
}

void initIDT() {
    // Set the ISR1 interrupt in the IDT
    idt[1].base_Lower = (base & 0xFFFF);
    idt[1].base_Higher = (base >> 16) & 0xFFFF;
    idt[1].selector = 0x08;
    idt[1].zero = 0;
    idt[1].flags = 0x8e;

    picRemap();  // Remap the PIC

    outportb(0x21, 0xfd);  // Mask all IRQs except for IRQ1 (keyboard)
    outportb(0xa1, 0xff);

    loadIdt();  // Load the IDT with the interrupt service routines
}

unsigned char inportb(unsigned short port) {
    unsigned char rv;
    __asm__ __volatile__ ("inb %1, %0" : "=a" (rv) : "dN" (port));
    return rv;
}

void outportb(unsigned short port, unsigned char data) {
    __asm__ __volatile__ ("outb %1, %0" : : "dN" (port), "a" (data));
}

extern void isr1_Handler() {
    // Handle keypress interrupt
    handleKeypress(inportb(0x60));  // Read scancode from the keyboard
    outportb(0x20, 0x20);  // Send End of Interrupt (EOI) signal to PIC1
    outportb(0xa0, 0x20);  // Send EOI to PIC2
}

void handleKeypress(int scancode) {
    // Map scancodes to characters
    char Scancode[] = {
        0, 0, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', 0, 0,
        'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '[', ']', 0, 0,
        'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ';', '\'', '`', 0, '\\',
        'Z', 'X', 'C', 'V', 'B', 'N', 'M', ',', '.', '/', 0, '*', 0, ' '
    };

    if (scancode == 0x1c)  // Enter key
        printChar('\n');
    else if (scancode < 0x3a)
        pressed(Scancode[scancode]);
}

void pressed(char key) {
    printChar(key);  // Print the pressed key
}

void picRemap() {
    // Save masks
    unsigned char a = inportb(PIC1_D);
    unsigned char b = inportb(PIC2_D);

    // Start the initialization sequence in cascade mode
    outportb(PIC1_C, ICW1_DEF | ICW1_ICW4);
    outportb(PIC2_C, ICW1_DEF | ICW1_ICW4);

    // Set PIC vector offsets
    outportb(PIC1_D, 0);  // Set offset for PIC1
    outportb(PIC2_D, 8);  // Set offset for PIC2

    // Tell PIC1 that there is a PIC2 at IRQ2
    outportb(PIC1_D, 4);
    // Tell PIC2 its cascade identity
    outportb(PIC2_D, 2);

    // Set x86 mode
    outportb(PIC1_D, ICW4_x86);
    outportb(PIC2_D, ICW4_x86);

    // Restore saved masks
    outportb(PIC1_D, a);
    outportb(PIC2_D, b);
}
