/* 
 * File:   main.c
 * Author: Mark David Harrington
 * code to illustrate how to use the bcm2835 with GTK 3  and Raspberry pi for IOT
 * applications  across ssh with X11 forwarding 
 *
 *  Created on 23 February 2020, 15:53
 *  Add this line to properties-> build -> complier -> additional properties below
    `pkg-config --cflags gtk+-3.0`
 *  Add this line to properties -> build -> linker -> additional properties  below
    `pkg-config --libs gtk+-3.0`
 *  Then click apply  then click OK 
 *  Now  your ready to compile GTK  windows software for Linux / Unix  
 * 
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <gtk/gtk.h>
#include <bcm2835.h>

/* 
 * simple gtk application 
 * MD Harrington 
 */

#define PIN RPI_GPIO_P1_11


/*global variable declaration*/

    GtkWidget *window; /* window */
    GtkWidget *btn;    /*Button which we will send signals from */
    GtkWidget *offimage ;
    GtkWidget *onimage  ;
    int gv_count = 0 ;  


/* this is our event handler for button clicked */
void on_btn1_clicked(GtkWidget *widget, gpointer data) {
    
  
  
  
  if(gv_count > 1)   
  {
      gv_count = 0 ;
  }
    
  
  
  if(gv_count == 0){
  bcm2835_gpio_write(PIN, HIGH);
  onimage = gtk_image_new_from_file("lightsOn.png") ;
  gtk_button_set_image (GTK_BUTTON (btn),onimage) ;
  }
  
  
  if(gv_count==1 )
  {
    bcm2835_gpio_write(PIN, LOW);  
    offimage = gtk_image_new_from_file("lightsoff.png") ;
    gtk_button_set_image (GTK_BUTTON (btn),offimage); 
  }
  
   gv_count+=1 ;
  
}

/* another callback */
void on_main_window_destroy(GtkWidget *widget, gpointer *data)
{   bcm2835_gpio_write(PIN, LOW); 
    bcm2835_close();
    gtk_main_quit ();
}


int main(int argc, char** argv) {

    GtkBuilder  *builder; 
    
    /* init hardware */
    
    if (!bcm2835_init())
      return 1;
     // Set the pin to be an output
    bcm2835_gpio_fsel(PIN, BCM2835_GPIO_FSEL_OUTP);
    
    // pass main method params to  gtkinit 
        
    gtk_init(&argc, &argv);

    // builder instance  and return pointer address
    
     builder = gtk_builder_new();
    // grab the galde exml file snd read this in 
     gtk_builder_add_from_file (builder, "switch.glade", NULL);
      
    // return the window object 
    
    window = GTK_WIDGET(gtk_builder_get_object(builder, "main_window")); 
 
     // now read the signals slots that we created in the glade file 
     
    gtk_builder_connect_signals(builder, NULL);
 
    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
    

    // get pointers to the buttons
    btn = GTK_WIDGET(gtk_builder_get_object(builder, "btn1"));
         
      
    /*show window with components */
    gtk_widget_show_all(window);
 
    /* This is the message event loop which continously monitors the  all events dispatched by
     * buttons , any other widgets inside the windows  and window events 
     */
    
    gtk_main();  
   
    return (EXIT_SUCCESS);
}
