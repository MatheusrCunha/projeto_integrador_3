

void lcd_init(void);   //Inicializa LCD

void lcd_send_cmd(char cmd);  //Enviar comando para o lcd

void lcd_send_data(char data);  

void lcd_send_string(char *str);  //enviar os caracteres para o lcd

void lcd_put_cur(int row, int col);

void lcd_clear(void); //Limpa a tela do lcd
