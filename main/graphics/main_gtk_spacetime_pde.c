#include <alloca.h>
#include <errno.h>
#include <stdlib.h>

#include <gtk/gtk.h>

#include "double.h"
#include "gtk_spacetime_pde.h"
#include "int.h"
#include "mesh.h"

static void do_drawing(GtkWidget * widget, cairo_t * cr, heat_flow * a)
{
  int height, n, width;
  int * i;
  GtkWidget * window;

  window = gtk_widget_get_toplevel(widget);
  gtk_window_get_size(GTK_WINDOW(window), &width, &height);
  gtk_spacetime_pde(cr, width, height, a);
  i = heat_flow_get_index(a);
  n = heat_flow_total_steps(a);
  if (*i < n - 1)
    *i += 1;
}

static int on_draw_event(GtkWidget * widget, cairo_t * cr, void * user_data)
{
  do_drawing(widget, cr, (heat_flow *) user_data);
  return FALSE;
}

static int time_handler(GtkWidget * widget)
{
  gtk_widget_queue_draw(widget);
  return TRUE;
}

int main(int argc, char * argv[])
{
  char * m_format, * u_format;
  char * m_filename, * u_filename;
  int begin, n, steps, total_colors;
  int * i;
  double * u;
  mesh * m;
  heat_flow * a;
  GtkWidget * window;
  GtkWidget * drawing_area;
  
  errno = 0;
  
  if (argc != 6)
  {
    fprintf(stderr, 
      "Error during execution of function %s in file %s on line %d: "
      "number of command-line arguments must be 6\n",
      __func__, __FILE__,__LINE__);
    errno = EINVAL;
    goto end;
  }
  
  begin = 0;
  i = &begin;
  
  m_format = argv[1];
  m_filename = argv[2];
  m = mesh_fscan_by_name(m_filename, m_format);
  if (errno)
  {
    fprintf(stderr,
      "Error during execution of function %s in file %s on line %d: "
      "could not generate input mesh\n",
      __func__, __FILE__,__LINE__);
    goto end;
  }
  
  steps = int_sscan(argv[3]);
  if (errno)
  {
    fprintf(stderr,
      "Error during execution of function %s in file %s on line %d: "
      "unable to scan number of time steps\n",
       __func__, __FILE__,__LINE__);
    goto m_free;
  }
  n = steps + 1;
  
  u_format = argv[4];
  u_filename = argv[5];
  u = double_matrix_fscan_by_name(u_filename, n, m->cn[0], u_format);
  if (errno)
  {
    fprintf(stderr,
      "Error during execution of function %s in file %s on line %d: "
      "could not generate temperatures\n",
       __func__, __FILE__,__LINE__);
    goto m_free;
  }
  
  total_colors = 1000;
  
  a = (heat_flow *) alloca(heat_flow_size());
  heat_flow_set(a, i, n, m, u, total_colors);
  
  gtk_init(&argc, &argv);

  window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  drawing_area = gtk_drawing_area_new();
  gtk_container_add(GTK_CONTAINER (window), drawing_area);

  g_signal_connect(G_OBJECT(drawing_area), "draw",
                   G_CALLBACK(on_draw_event), a);
  g_signal_connect(G_OBJECT(window), "destroy",
                   G_CALLBACK(gtk_main_quit), NULL);

  gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
  gtk_window_set_default_size(GTK_WINDOW(window), 500, 500);
  gtk_window_set_title(GTK_WINDOW(window), "Heat flow in 2D");

  g_timeout_add(100, (GSourceFunc) time_handler, (void *) window);

  gtk_widget_show_all(window);
  gtk_main();
  errno = 0;
  
  free(u);
m_free:
  mesh_free(m);
end:
  return errno;
}
