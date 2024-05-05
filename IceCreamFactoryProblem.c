#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <stdbool.h>
#include <errno.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include <time.h>
#include <semaphore.h>

#define FlavorCount 3
#define ToppingCount 2
#define TicketCount 30

#define F1P 1.86
#define F2P 1.92
#define F3P 2.12

#define T1P 0.29
#define T2P 0.58

int tickets_remaining = TicketCount;
int flavors[FlavorCount] = {45, 52, 29};
int toppings[ToppingCount] = {42, 52};
double revenue = 0.0;
bool flag = false;

sem_t ticket_sem;
sem_t flavor_sem;
sem_t topping_sem;
sem_t revenue_sem;
sem_t text_sem;
sem_t loop_sem;
sem_t count_sem;

GtkWidget * text_view;
GtkWidget * window;
GtkWidget * image;
GtkCssProvider * cssProvider;

void append_text(const gchar * text) {
  sem_wait( & text_sem);
  GtkTextBuffer * buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_view));
  GtkTextIter iter;
  gtk_text_buffer_get_end_iter(buffer, & iter);
  gtk_text_buffer_insert(buffer, & iter, text, -1);

  sem_post( & text_sem);
}

void * iceCream(void * id) {
  srand(time(0));
  int customer_id = * ((int * ) id);
  double bill = 0.0;
  int count = 0;

  sem_wait( & ticket_sem);
  tickets_remaining--;
  append_text(g_strdup_printf("Customer[%d] Got Ticket.\n", customer_id));
  sem_post( & ticket_sem);

  usleep(1000000);

  sem_wait( & flavor_sem);

  int chosen_flavors[FlavorCount] = {0};
  sem_wait( & count_sem);
  while (count < 3) {
    if (flavors[0] > 0 || flavors[1] > 0 || flavors[2] > 0) {
      if (flavors[0] > 0 && flavors[1] > 0 && flavors[2] > 0) {
        int f = rand() % 3;
        if (flavors[f] > 0) {
          chosen_flavors[f] = 1;
          flavors[f]--;
          if (f == 0) {
            bill += F1P;
            flag = true;
          } else if (f == 1) {
            bill += F2P;
            flag = true;
          } else if (f == 2) {
            bill += F3P;
            flag = true;
          }
        }
        append_text(g_strdup_printf("Customer[%d]: Got Flavour[%d].\n", customer_id, f));
        count++;
      } else if (flavors[0] > 0 && flavors[1] > 0 && flavors[2] <= 0) {
        int x = rand() % 2;
        if (flavors[x] > 0) {
          chosen_flavors[x] = 1;
          flavors[x]--;
          if (x == 0) {
            bill += F1P;
            flag = true;
          } else if (x == 1) {
            bill += F2P;
            flag = true;
          }
        }
        append_text(g_strdup_printf("Customer[%d]: Got Flavour[%d].\n", customer_id, x));
        count++;
      } else if (flavors[0] > 0 && flavors[2] > 0 && flavors[1] <= 0) {
        int y = rand() % 2;
        if (y == 1) {
          y = 2;
        }
        if (flavors[y] > 0) {
          chosen_flavors[y] = 1;
          flavors[y]--;
          if (y == 0) {
            bill += F1P;
            flag = true;
          } else if (y == 2) {
            bill += F3P;
            flag = true;
          }
        }
        append_text(g_strdup_printf("Customer[%d]: Got Flavour[%d].\n", customer_id, y));
        count++;
      } else if (flavors[1] > 0 && flavors[2] > 0 && flavors[2] <= 0) {
        int z = rand() % 2;
        z++;
        if (flavors[z] > 0) {
          chosen_flavors[z] = 1;
          flavors[z]--;
          if (z == 1) {
            bill += F2P;
            flag = true;
          } else if (z == 1) {
            bill += F3P;
            flag = true;
          }
        }
        append_text(g_strdup_printf("Customer[%d]: Got Flavour[%d].\n", customer_id, z));
        count++;
      } else if (flavors[0] > 0 && flavors[1] <= 0 && flavors[2] <= 0) {
        if (flavors[0] > 0) {
          chosen_flavors[0] = 1;
          flavors[0]--;
          bill += F1P;
          flag = true;
        }
        append_text(g_strdup_printf("Customer[%d]: Got Flavour[%d].\n", customer_id, 0));
        count++;
      } else if (flavors[1] > 0 && flavors[0] <= 0 && flavors[2] <= 0) {
        if (flavors[1] > 0) {
          chosen_flavors[1] = 1;
          flavors[1]--;
          bill += F2P;
          flag = true;
        }
        append_text(g_strdup_printf("Customer[%d]: Got Flavour[%d].\n", customer_id, 1));
        count++;
      } else if (flavors[2] > 0 && flavors[0] <= 0 && flavors[1] <= 0) {
        if (flavors[2] > 0) {
          chosen_flavors[2] = 1;
          flavors[2]--;
          bill += F3P;
          flag = true;
        }
        append_text(g_strdup_printf("Customer[%d]: Got Flavour[%d].\n", customer_id, 2));
        count++;
      }
    } else {
      sem_wait( & loop_sem);
      int tops = toppings[0] + toppings[1];
      syscall(452, tops);
      sem_post( & loop_sem);
      break;
    }
  }
  sem_post( & count_sem);
  sem_post( & flavor_sem);

  int chosen_toppings[ToppingCount] = {
    0
  };
  sem_wait( & topping_sem);
  if (flag) {
    for (int i = 0; i < ToppingCount; i++) {
      if (toppings[i] > 0) {
        chosen_toppings[i] = 1;
        toppings[i]--;
        if (i == 0)
          bill += T1P;
        else if (i == 1)
          bill += T2P;
      }
    }
    flag = false;
  }
  sem_post( & topping_sem);

  sem_wait( & revenue_sem);
  revenue += bill;
  append_text(g_strdup_printf("Customer[%d]: Billed: $ %.2f.\n", customer_id, bill));
  sem_post( & revenue_sem);

  append_text(g_strdup_printf("Customer[%d]: Leaving Ice-Cream Shop.\n\n", customer_id));

  return NULL;
}

void on_window_destroy() {
  gtk_main_quit();
}

int main(int argc, char * argv[]) {
  int num_customers;
  printf("Enter Number Of Customers [1-%d]: ", TicketCount);
  scanf("%d", & num_customers);

  if (num_customers > TicketCount || num_customers < 1) {
    long int checkcall = syscall(453);
    printf("Exiting code\n");
    return 0;
  }
  int totalf = flavors[0] + flavors[1] + flavors[2];

  sem_init( & ticket_sem, 0, 1);
  sem_init( & flavor_sem, 0, 1);
  sem_init( & topping_sem, 0, 1);
  sem_init( & revenue_sem, 0, 1);
  sem_init( & text_sem, 0, 1);
  sem_init( & loop_sem, 0, 1);
  sem_init( & count_sem, 0, 1);

  gtk_init( & argc, & argv);

  window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title(GTK_WINDOW(window), "Ice Cream Shop");
  gtk_container_set_border_width(GTK_CONTAINER(window), 125);
  gtk_window_set_default_size(GTK_WINDOW(window), 1280, 720);

  text_view = gtk_text_view_new();
  gtk_text_view_set_editable(GTK_TEXT_VIEW(text_view), FALSE);
  gtk_text_view_set_cursor_visible(GTK_TEXT_VIEW(text_view), FALSE);
  GtkWidget * scroll_window = gtk_scrolled_window_new(NULL, NULL);
  gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scroll_window), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
  gtk_container_add(GTK_CONTAINER(scroll_window), text_view);
  gtk_container_add(GTK_CONTAINER(window), scroll_window);
  GdkColor color;
  gdk_color_parse("#F4C2C2", & color);
  gtk_widget_modify_bg(GTK_WIDGET(text_view), GTK_STATE_NORMAL, & color);

  image = gtk_image_new_from_file("1.jpg");
  gtk_container_add(GTK_CONTAINER(window), image);

  cssProvider = gtk_css_provider_new();
  gtk_css_provider_load_from_path(cssProvider, "style.css", NULL);
  gtk_style_context_add_provider_for_screen(gdk_screen_get_default(), GTK_STYLE_PROVIDER(cssProvider), GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);

  gtk_widget_show_all(window);

  g_signal_connect(window, "destroy", G_CALLBACK(on_window_destroy), NULL);

  pthread_t threads[num_customers];
  int customer_ids[num_customers];
  for (int i = 0; i < num_customers; i++) {
    customer_ids[i] = i + 1;
    pthread_create( & threads[i], NULL, iceCream, & customer_ids[i]);
  }

  for (int i = 0; i < num_customers; i++) {
    pthread_join(threads[i], NULL);
  }

  gtk_main();
  int totals = flavors[0] + flavors[1] + flavors[2];
  totals = totalf - totals;
  int a = (int) revenue;
  int b = (revenue * 100) - (a * 100);
  int tick = tickets_remaining;

  long int ca = syscall(455, num_customers, tick, a, b);
  long int cb = syscall(456, totals, flavors[0], flavors[1], flavors[2]);
  long int checks = syscall(454);
  printf("Good Bye\n");

  return 0;
}
