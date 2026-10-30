#include "Cal.h"
#include<ctype.h>

#define ADD_CHARACTER(text, chr) text[strlen(text)] = chr
#define ADD_WORD(text,word) strcpy(text + strlen(text), word)
#define LAST_CHARACTER(text) text[strlen(text) - 1]

#include<gtk/gtk.h>

GtkWidget *window = NULL;

GtkWidget *expression = NULL;
char expressionString[maxExpressionLength];

void calculateHandler(GtkWidget *widget,GParamSpec *pspec, gpointer user_data){
	//Function to calculate the result of an expression
	//Function meant to be executed at every update of the expression label
	
	#define answerLength 1000
	
	char expr[maxExpressionLength];
	strcpy(expr,expressionString);
	
	double answer;
	char stringAnswer[answerLength];
	
	if(calculate(expr,&answer) == true){
		//If the answer is too big or small , we write it in standard form
		if(fabs(answer) >= pow(10,10) || fabs(answer) <= pow(10,-9)){
			sprintf(stringAnswer,"%.9e",answer);
		}else{
			sprintf(stringAnswer,"%.9lf",answer);
		}
	}else{
		sprintf(stringAnswer," ");
	}
	gtk_label_set_text(GTK_LABEL(user_data), stringAnswer);
}

void CEhandler(GtkWidget *widget, gpointer user_data){
	//Handles the button CE
	strset(expressionString,0);
	gtk_label_set_text(GTK_LABEL(user_data), " ");
}

void EXPhandler(GtkWidget *widget, gpointer user_data){
	//Handles the button EXP
	ADD_WORD(expressionString, " e ");
	gtk_label_set_text(GTK_LABEL(user_data), expressionString);
}

void MinusSignHandler(GtkWidget *widget, gpointer user_data){
	//Handles the button +/-
	ADD_CHARACTER(expressionString, '-');
	gtk_label_set_text(GTK_LABEL(user_data), expressionString);
}

void StackSwitchHandler(GtkWidget *widget, gpointer user_data){
	//Handles the button "F"
	GList *pages = gtk_container_get_children(GTK_CONTAINER(user_data));
	
	GtkWidget *firstChild = GTK_WIDGET(g_list_nth_data(pages, 0));
	GtkWidget *secondChild = GTK_WIDGET(g_list_nth_data(pages, 1));
	
	if(gtk_stack_get_visible_child(GTK_STACK(user_data)) == firstChild){
		gtk_stack_set_visible_child(GTK_STACK(user_data), secondChild);
	}else{
		gtk_stack_set_visible_child(GTK_STACK(user_data), firstChild);
	}
}

void ButtonClickedHandler(GtkWidget *widget, gpointer user_data){
	//Handles all other buttons
	char *entry = (char*)gtk_label_get_text(GTK_LABEL(user_data));
	
	//Functions and signs come with spaces around them. numbers do not
	if(ispunct(entry[0]) && entry[0] != ',' || isalpha(entry[0])){
		if(LAST_CHARACTER(expressionString) != ' '){
			ADD_CHARACTER(expressionString, ' ');
		}
		ADD_WORD(expressionString, entry);
		ADD_CHARACTER(expressionString, ' ');
	}else{
		ADD_CHARACTER(expressionString, entry[0]);
	}
	gtk_label_set_text(GTK_LABEL(expression), expressionString);
}

int main(int argc, char *argv[]){
	gtk_init(&argc,&argv);
	
	GtkBuilder *builder = gtk_builder_new();
	
	//We define the interface through a string, which we will pass to a builder
	const gchar *interface = "<interface>\
  <requires lib=\"gtk+\" version=\"3.24\"/>\
  <object class=\"GtkWindow\" id=\"mainWindow\">\
    <property name=\"can-focus\">False</property>\
    <property name=\"title\" translatable=\"yes\">Calculator Prototype</property>\
    <property name=\"resizable\">False</property>\
    <property name=\"default-width\">360</property>\
    <property name=\"default-height\">540</property>\
    <property name=\"icon\">CalculatorIcon.png</property>\
    <signal name=\"destroy\" handler=\"gtk_main_quit\" swapped=\"no\"/>\
    <child>\
      <!-- n-columns=5 n-rows=8 -->\
      <object class=\"GtkGrid\" id=\"layout grid\">\
        <property name=\"visible\">True</property>\
        <property name=\"can-focus\">False</property>\
        <property name=\"row-homogeneous\">True</property>\
        <property name=\"column-homogeneous\">True</property>\
        <child>\
          <object class=\"GtkStack\" id=\"number/function stack\">\
            <property name=\"visible\">True</property>\
            <property name=\"can-focus\">False</property>\
            <property name=\"transition-duration\">0</property>\
            <child>\
              <!-- n-columns=3 n-rows=5 -->\
              <object class=\"GtkGrid\" id=\"first page grid\">\
                <property name=\"visible\">True</property>\
                <property name=\"can-focus\">False</property>\
                <property name=\"row-homogeneous\">True</property>\
                <property name=\"column-homogeneous\">True</property>\
                <child>\
                  <object class=\"GtkButton\" id=\"7 button\">\
                    <property name=\"visible\">True</property>\
                    <property name=\"can-focus\">True</property>\
                    <property name=\"receives-default\">True</property>\
                    <signal name=\"clicked\" handler=\"ButtonClickedHandler\" object=\"7\" swapped=\"no\"/>\
                    <child>\
                      <object class=\"GtkLabel\" id=\"7\">\
                        <property name=\"visible\">True</property>\
                        <property name=\"can-focus\">False</property>\
                        <property name=\"label\" translatable=\"yes\">7</property>\
                        <attributes>\
                          <attribute name=\"font-desc\" value=\"Segoe UI Bold 20\"/>\
                          <attribute name=\"style\" value=\"oblique\"/>\
                        </attributes>\
                      </object>\
                    </child>\
                  </object>\
                  <packing>\
                    <property name=\"left-attach\">0</property>\
                    <property name=\"top-attach\">1</property>\
                  </packing>\
                </child>\
                <child>\
                  <object class=\"GtkButton\" id=\"8 button\">\
                    <property name=\"visible\">True</property>\
                    <property name=\"can-focus\">True</property>\
                    <property name=\"receives-default\">True</property>\
                    <signal name=\"clicked\" handler=\"ButtonClickedHandler\" object=\"8\" swapped=\"no\"/>\
                    <child>\
                      <object class=\"GtkLabel\" id=\"8\">\
                        <property name=\"visible\">True</property>\
                        <property name=\"can-focus\">False</property>\
                        <property name=\"label\" translatable=\"yes\">8</property>\
                        <attributes>\
                          <attribute name=\"font-desc\" value=\"Segoe UI Bold 20\"/>\
                          <attribute name=\"style\" value=\"oblique\"/>\
                        </attributes>\
                      </object>\
                    </child>\
                  </object>\
                  <packing>\
                    <property name=\"left-attach\">1</property>\
                    <property name=\"top-attach\">1</property>\
                  </packing>\
                </child>\
                <child>\
                  <object class=\"GtkButton\" id=\"9 button\">\
                    <property name=\"visible\">True</property>\
                    <property name=\"can-focus\">True</property>\
                    <property name=\"receives-default\">True</property>\
                    <signal name=\"clicked\" handler=\"ButtonClickedHandler\" object=\"9\" swapped=\"no\"/>\
                    <child>\
                      <object class=\"GtkLabel\" id=\"9\">\
                        <property name=\"visible\">True</property>\
                        <property name=\"can-focus\">False</property>\
                        <property name=\"label\" translatable=\"yes\">9</property>\
                        <attributes>\
                          <attribute name=\"font-desc\" value=\"Segoe UI Bold 20\"/>\
                          <attribute name=\"style\" value=\"oblique\"/>\
                        </attributes>\
                      </object>\
                    </child>\
                  </object>\
                  <packing>\
                    <property name=\"left-attach\">2</property>\
                    <property name=\"top-attach\">1</property>\
                  </packing>\
                </child>\
                <child>\
                  <object class=\"GtkButton\" id=\"4 button\">\
                    <property name=\"visible\">True</property>\
                    <property name=\"can-focus\">True</property>\
                    <property name=\"receives-default\">True</property>\
                    <signal name=\"clicked\" handler=\"ButtonClickedHandler\" object=\"4\" swapped=\"no\"/>\
                    <child>\
                      <object class=\"GtkLabel\" id=\"4\">\
                        <property name=\"visible\">True</property>\
                        <property name=\"can-focus\">False</property>\
                        <property name=\"label\" translatable=\"yes\">4</property>\
                        <attributes>\
                          <attribute name=\"font-desc\" value=\"Segoe UI Bold 20\"/>\
                          <attribute name=\"style\" value=\"oblique\"/>\
                        </attributes>\
                      </object>\
                    </child>\
                  </object>\
                  <packing>\
                    <property name=\"left-attach\">0</property>\
                    <property name=\"top-attach\">2</property>\
                  </packing>\
                </child>\
                <child>\
                  <object class=\"GtkButton\" id=\"6 button\">\
                    <property name=\"visible\">True</property>\
                    <property name=\"can-focus\">True</property>\
                    <property name=\"receives-default\">True</property>\
                    <signal name=\"clicked\" handler=\"ButtonClickedHandler\" object=\"6\" swapped=\"no\"/>\
                    <child>\
                      <object class=\"GtkLabel\" id=\"6\">\
                        <property name=\"visible\">True</property>\
                        <property name=\"can-focus\">False</property>\
                        <property name=\"label\" translatable=\"yes\">6</property>\
                        <attributes>\
                          <attribute name=\"font-desc\" value=\"Segoe UI Bold 20\"/>\
                          <attribute name=\"style\" value=\"oblique\"/>\
                        </attributes>\
                      </object>\
                    </child>\
                  </object>\
                  <packing>\
                    <property name=\"left-attach\">2</property>\
                    <property name=\"top-attach\">2</property>\
                  </packing>\
                </child>\
                <child>\
                  <object class=\"GtkButton\" id=\"5 button\">\
                    <property name=\"visible\">True</property>\
                    <property name=\"can-focus\">True</property>\
                    <property name=\"receives-default\">True</property>\
                    <signal name=\"clicked\" handler=\"ButtonClickedHandler\" object=\"5\" swapped=\"no\"/>\
                    <child>\
                      <object class=\"GtkLabel\" id=\"5\">\
                        <property name=\"visible\">True</property>\
                        <property name=\"can-focus\">False</property>\
                        <property name=\"label\" translatable=\"yes\">5</property>\
                        <attributes>\
                          <attribute name=\"font-desc\" value=\"Segoe UI Bold 20\"/>\
                          <attribute name=\"style\" value=\"oblique\"/>\
                        </attributes>\
                      </object>\
                    </child>\
                  </object>\
                  <packing>\
                    <property name=\"left-attach\">1</property>\
                    <property name=\"top-attach\">2</property>\
                  </packing>\
                </child>\
                <child>\
                  <object class=\"GtkButton\" id=\"1 button\">\
                    <property name=\"visible\">True</property>\
                    <property name=\"can-focus\">True</property>\
                    <property name=\"receives-default\">True</property>\
                    <signal name=\"clicked\" handler=\"ButtonClickedHandler\" object=\"1\" swapped=\"no\"/>\
                    <child>\
                      <object class=\"GtkLabel\" id=\"1\">\
                        <property name=\"visible\">True</property>\
                        <property name=\"can-focus\">False</property>\
                        <property name=\"label\" translatable=\"yes\">1</property>\
                        <attributes>\
                          <attribute name=\"font-desc\" value=\"Segoe UI Bold 20\"/>\
                          <attribute name=\"style\" value=\"oblique\"/>\
                        </attributes>\
                      </object>\
                    </child>\
                  </object>\
                  <packing>\
                    <property name=\"left-attach\">0</property>\
                    <property name=\"top-attach\">3</property>\
                  </packing>\
                </child>\
                <child>\
                  <object class=\"GtkButton\" id=\"2 button\">\
                    <property name=\"visible\">True</property>\
                    <property name=\"can-focus\">True</property>\
                    <property name=\"receives-default\">True</property>\
                    <signal name=\"clicked\" handler=\"ButtonClickedHandler\" object=\"2\" swapped=\"no\"/>\
                    <child>\
                      <object class=\"GtkLabel\" id=\"2\">\
                        <property name=\"visible\">True</property>\
                        <property name=\"can-focus\">False</property>\
                        <property name=\"label\" translatable=\"yes\">2</property>\
                        <attributes>\
                          <attribute name=\"font-desc\" value=\"Segoe UI Bold 20\"/>\
                          <attribute name=\"style\" value=\"oblique\"/>\
                        </attributes>\
                      </object>\
                    </child>\
                  </object>\
                  <packing>\
                    <property name=\"left-attach\">1</property>\
                    <property name=\"top-attach\">3</property>\
                  </packing>\
                </child>\
                <child>\
                  <object class=\"GtkButton\" id=\"3 button\">\
                    <property name=\"visible\">True</property>\
                    <property name=\"can-focus\">True</property>\
                    <property name=\"receives-default\">True</property>\
                    <signal name=\"clicked\" handler=\"ButtonClickedHandler\" object=\"3\" swapped=\"no\"/>\
                    <child>\
                      <object class=\"GtkLabel\" id=\"3\">\
                        <property name=\"visible\">True</property>\
                        <property name=\"can-focus\">False</property>\
                        <property name=\"label\" translatable=\"yes\">3</property>\
                        <attributes>\
                          <attribute name=\"font-desc\" value=\"Segoe UI Bold 20\"/>\
                          <attribute name=\"style\" value=\"oblique\"/>\
                        </attributes>\
                      </object>\
                    </child>\
                  </object>\
                  <packing>\
                    <property name=\"left-attach\">2</property>\
                    <property name=\"top-attach\">3</property>\
                  </packing>\
                </child>\
                <child>\
                  <object class=\"GtkButton\" id=\"0 button\">\
                    <property name=\"visible\">True</property>\
                    <property name=\"can-focus\">True</property>\
                    <property name=\"receives-default\">True</property>\
                    <signal name=\"clicked\" handler=\"ButtonClickedHandler\" object=\"0\" swapped=\"no\"/>\
                    <child>\
                      <object class=\"GtkLabel\" id=\"0\">\
                        <property name=\"visible\">True</property>\
                        <property name=\"can-focus\">False</property>\
                        <property name=\"label\" translatable=\"yes\">0</property>\
                        <attributes>\
                          <attribute name=\"font-desc\" value=\"Segoe UI Bold 20\"/>\
                          <attribute name=\"style\" value=\"oblique\"/>\
                        </attributes>\
                      </object>\
                    </child>\
                  </object>\
                  <packing>\
                    <property name=\"left-attach\">0</property>\
                    <property name=\"top-attach\">4</property>\
                  </packing>\
                </child>\
                <child>\
                  <object class=\"GtkButton\" id=\"+/- button\">\
                    <property name=\"visible\">True</property>\
                    <property name=\"can-focus\">True</property>\
                    <property name=\"receives-default\">True</property>\
                    <signal name=\"clicked\" handler=\"MinusSignHandler\" object=\"expression label\" swapped=\"no\"/>\
                    <child>\
                      <object class=\"GtkLabel\" id=\"+/-\">\
                        <property name=\"visible\">True</property>\
                        <property name=\"can-focus\">False</property>\
                        <property name=\"label\" translatable=\"yes\">+/-</property>\
                        <attributes>\
                          <attribute name=\"font-desc\" value=\"Segoe UI Bold 13\"/>\
                          <attribute name=\"style\" value=\"oblique\"/>\
                          <attribute name=\"weight\" value=\"ultraheavy\"/>\
                        </attributes>\
                      </object>\
                    </child>\
                  </object>\
                  <packing>\
                    <property name=\"left-attach\">1</property>\
                    <property name=\"top-attach\">4</property>\
                  </packing>\
                </child>\
                <child>\
                  <object class=\"GtkButton\" id=\". button\">\
                    <property name=\"visible\">True</property>\
                    <property name=\"can-focus\">True</property>\
                    <property name=\"receives-default\">True</property>\
                    <signal name=\"clicked\" handler=\"ButtonClickedHandler\" object=\".\" swapped=\"no\"/>\
                    <child>\
                      <object class=\"GtkLabel\" id=\".\">\
                        <property name=\"visible\">True</property>\
                        <property name=\"can-focus\">False</property>\
                        <property name=\"label\" translatable=\"yes\">,</property>\
                        <property name=\"yalign\">0</property>\
                        <attributes>\
                          <attribute name=\"font-desc\" value=\"Segoe UI Bold 25\"/>\
                          <attribute name=\"style\" value=\"oblique\"/>\
                          <attribute name=\"weight\" value=\"ultraheavy\"/>\
                        </attributes>\
                      </object>\
                    </child>\
                  </object>\
                  <packing>\
                    <property name=\"left-attach\">2</property>\
                    <property name=\"top-attach\">4</property>\
                  </packing>\
                </child>\
                <child>\
                  <object class=\"GtkButton\" id=\"( button\">\
                    <property name=\"visible\">True</property>\
                    <property name=\"can-focus\">True</property>\
                    <property name=\"receives-default\">True</property>\
                    <signal name=\"clicked\" handler=\"ButtonClickedHandler\" object=\"(\" swapped=\"no\"/>\
                    <child>\
                      <object class=\"GtkLabel\" id=\"(\">\
                        <property name=\"visible\">True</property>\
                        <property name=\"can-focus\">False</property>\
                        <property name=\"label\" translatable=\"yes\">(</property>\
                        <attributes>\
                          <attribute name=\"font-desc\" value=\"Segoe UI Bold 20\"/>\
                          <attribute name=\"style\" value=\"oblique\"/>\
                        </attributes>\
                      </object>\
                    </child>\
                  </object>\
                  <packing>\
                    <property name=\"left-attach\">0</property>\
                    <property name=\"top-attach\">0</property>\
                  </packing>\
                </child>\
                <child>\
                  <object class=\"GtkButton\" id=\") button\">\
                    <property name=\"visible\">True</property>\
                    <property name=\"can-focus\">True</property>\
                    <property name=\"receives-default\">True</property>\
                    <signal name=\"clicked\" handler=\"ButtonClickedHandler\" object=\")\" swapped=\"no\"/>\
                    <child>\
                      <object class=\"GtkLabel\" id=\")\">\
                        <property name=\"visible\">True</property>\
                        <property name=\"can-focus\">False</property>\
                        <property name=\"label\" translatable=\"yes\">)</property>\
                        <attributes>\
                          <attribute name=\"font-desc\" value=\"Segoe UI Bold 20\"/>\
                          <attribute name=\"style\" value=\"oblique\"/>\
                        </attributes>\
                      </object>\
                    </child>\
                  </object>\
                  <packing>\
                    <property name=\"left-attach\">1</property>\
                    <property name=\"top-attach\">0</property>\
                  </packing>\
                </child>\
                <child>\
                  <object class=\"GtkButton\" id=\"EXP button\">\
                    <property name=\"visible\">True</property>\
                    <property name=\"can-focus\">True</property>\
                    <property name=\"receives-default\">True</property>\
                    <signal name=\"clicked\" handler=\"EXPhandler\" object=\"expression label\" swapped=\"no\"/>\
                    <child>\
                      <object class=\"GtkLabel\" id=\"EXP\">\
                        <property name=\"visible\">True</property>\
                        <property name=\"can-focus\">False</property>\
                        <property name=\"label\" translatable=\"yes\">EXP</property>\
                        <attributes>\
                          <attribute name=\"font-desc\" value=\"Segoe UI Semi-Bold 20\"/>\
                          <attribute name=\"style\" value=\"italic\"/>\
                        </attributes>\
                      </object>\
                    </child>\
                  </object>\
                  <packing>\
                    <property name=\"left-attach\">2</property>\
                    <property name=\"top-attach\">0</property>\
                  </packing>\
                </child>\
              </object>\
              <packing>\
                <property name=\"name\">page0</property>\
                <property name=\"title\" translatable=\"yes\">page0</property>\
              </packing>\
            </child>\
            <child>\
              <!-- n-columns=3 n-rows=8 -->\
              <object class=\"GtkGrid\" id=\"second page grid\">\
                <property name=\"visible\">True</property>\
                <property name=\"can-focus\">False</property>\
                <property name=\"row-homogeneous\">True</property>\
                <property name=\"column-homogeneous\">True</property>\
                <child>\
                  <object class=\"GtkButton\" id=\"sin button\">\
                    <property name=\"visible\">True</property>\
                    <property name=\"can-focus\">True</property>\
                    <property name=\"receives-default\">True</property>\
                    <signal name=\"clicked\" handler=\"ButtonClickedHandler\" object=\"sin\" swapped=\"no\"/>\
                    <child>\
                      <object class=\"GtkLabel\" id=\"sin\">\
                        <property name=\"visible\">True</property>\
                        <property name=\"can-focus\">False</property>\
                        <property name=\"label\" translatable=\"yes\">sin</property>\
                        <attributes>\
                          <attribute name=\"font-desc\" value=\"Segoe UI Bold 15\"/>\
                          <attribute name=\"style\" value=\"oblique\"/>\
                        </attributes>\
                      </object>\
                    </child>\
                  </object>\
                  <packing>\
                    <property name=\"left-attach\">0</property>\
                    <property name=\"top-attach\">0</property>\
                  </packing>\
                </child>\
                <child>\
                  <object class=\"GtkButton\" id=\"cos button\">\
                    <property name=\"visible\">True</property>\
                    <property name=\"can-focus\">True</property>\
                    <property name=\"receives-default\">True</property>\
                    <signal name=\"clicked\" handler=\"ButtonClickedHandler\" object=\"cos\" swapped=\"no\"/>\
                    <child>\
                      <object class=\"GtkLabel\" id=\"cos\">\
                        <property name=\"visible\">True</property>\
                        <property name=\"can-focus\">False</property>\
                        <property name=\"label\" translatable=\"yes\">cos</property>\
                        <attributes>\
                          <attribute name=\"font-desc\" value=\"Segoe UI Bold 15\"/>\
                          <attribute name=\"style\" value=\"oblique\"/>\
                        </attributes>\
                      </object>\
                    </child>\
                  </object>\
                  <packing>\
                    <property name=\"left-attach\">1</property>\
                    <property name=\"top-attach\">0</property>\
                  </packing>\
                </child>\
                <child>\
                  <object class=\"GtkButton\" id=\"acos button\">\
                    <property name=\"visible\">True</property>\
                    <property name=\"can-focus\">True</property>\
                    <property name=\"receives-default\">True</property>\
                    <signal name=\"clicked\" handler=\"ButtonClickedHandler\" object=\"acos\" swapped=\"no\"/>\
                    <child>\
                      <object class=\"GtkLabel\" id=\"acos\">\
                        <property name=\"visible\">True</property>\
                        <property name=\"can-focus\">False</property>\
                        <property name=\"label\" translatable=\"yes\">acos</property>\
                        <attributes>\
                          <attribute name=\"font-desc\" value=\"Segoe UI Bold 15\"/>\
                          <attribute name=\"style\" value=\"oblique\"/>\
                        </attributes>\
                      </object>\
                    </child>\
                  </object>\
                  <packing>\
                    <property name=\"left-attach\">1</property>\
                    <property name=\"top-attach\">1</property>\
                  </packing>\
                </child>\
                <child>\
                  <object class=\"GtkButton\" id=\"tan button\">\
                    <property name=\"visible\">True</property>\
                    <property name=\"can-focus\">True</property>\
                    <property name=\"receives-default\">True</property>\
                    <signal name=\"clicked\" handler=\"ButtonClickedHandler\" object=\"tan\" swapped=\"no\"/>\
                    <child>\
                      <object class=\"GtkLabel\" id=\"tan\">\
                        <property name=\"visible\">True</property>\
                        <property name=\"can-focus\">False</property>\
                        <property name=\"label\" translatable=\"yes\">tan</property>\
                        <attributes>\
                          <attribute name=\"font-desc\" value=\"Segoe UI Bold 15\"/>\
                          <attribute name=\"style\" value=\"oblique\"/>\
                        </attributes>\
                      </object>\
                    </child>\
                  </object>\
                  <packing>\
                    <property name=\"left-attach\">2</property>\
                    <property name=\"top-attach\">0</property>\
                  </packing>\
                </child>\
                <child>\
                  <object class=\"GtkButton\" id=\"asin button\">\
                    <property name=\"visible\">True</property>\
                    <property name=\"can-focus\">True</property>\
                    <property name=\"receives-default\">True</property>\
                    <signal name=\"clicked\" handler=\"ButtonClickedHandler\" object=\"asin\" swapped=\"no\"/>\
                    <child>\
                      <object class=\"GtkLabel\" id=\"asin\">\
                        <property name=\"visible\">True</property>\
                        <property name=\"can-focus\">False</property>\
                        <property name=\"label\" translatable=\"yes\">asin</property>\
                        <attributes>\
                          <attribute name=\"font-desc\" value=\"Segoe UI Bold 15\"/>\
                          <attribute name=\"style\" value=\"oblique\"/>\
                        </attributes>\
                      </object>\
                    </child>\
                  </object>\
                  <packing>\
                    <property name=\"left-attach\">0</property>\
                    <property name=\"top-attach\">1</property>\
                  </packing>\
                </child>\
                <child>\
                  <object class=\"GtkButton\" id=\"sinh button\">\
                    <property name=\"visible\">True</property>\
                    <property name=\"can-focus\">True</property>\
                    <property name=\"receives-default\">True</property>\
                    <signal name=\"clicked\" handler=\"ButtonClickedHandler\" object=\"sinh\" swapped=\"no\"/>\
                    <child>\
                      <object class=\"GtkLabel\" id=\"sinh\">\
                        <property name=\"visible\">True</property>\
                        <property name=\"can-focus\">False</property>\
                        <property name=\"label\" translatable=\"yes\">sinh</property>\
                        <attributes>\
                          <attribute name=\"font-desc\" value=\"Segoe UI Bold 15\"/>\
                          <attribute name=\"style\" value=\"oblique\"/>\
                        </attributes>\
                      </object>\
                    </child>\
                  </object>\
                  <packing>\
                    <property name=\"left-attach\">0</property>\
                    <property name=\"top-attach\">2</property>\
                  </packing>\
                </child>\
                <child>\
                  <object class=\"GtkButton\" id=\"atan button\">\
                    <property name=\"visible\">True</property>\
                    <property name=\"can-focus\">True</property>\
                    <property name=\"receives-default\">True</property>\
                    <signal name=\"clicked\" handler=\"ButtonClickedHandler\" object=\"atan\" swapped=\"no\"/>\
                    <child>\
                      <object class=\"GtkLabel\" id=\"atan\">\
                        <property name=\"visible\">True</property>\
                        <property name=\"can-focus\">False</property>\
                        <property name=\"label\" translatable=\"yes\">atan</property>\
                        <attributes>\
                          <attribute name=\"font-desc\" value=\"Segoe UI Bold 15\"/>\
                          <attribute name=\"style\" value=\"oblique\"/>\
                        </attributes>\
                      </object>\
                    </child>\
                  </object>\
                  <packing>\
                    <property name=\"left-attach\">2</property>\
                    <property name=\"top-attach\">1</property>\
                  </packing>\
                </child>\
                <child>\
                  <object class=\"GtkButton\" id=\"cosh button\">\
                    <property name=\"visible\">True</property>\
                    <property name=\"can-focus\">True</property>\
                    <property name=\"receives-default\">True</property>\
                    <signal name=\"clicked\" handler=\"ButtonClickedHandler\" object=\"cosh\" swapped=\"no\"/>\
                    <child>\
                      <object class=\"GtkLabel\" id=\"cosh\">\
                        <property name=\"visible\">True</property>\
                        <property name=\"can-focus\">False</property>\
                        <property name=\"label\" translatable=\"yes\">cosh</property>\
                        <attributes>\
                          <attribute name=\"font-desc\" value=\"Segoe UI Bold 15\"/>\
                          <attribute name=\"style\" value=\"oblique\"/>\
                        </attributes>\
                      </object>\
                    </child>\
                  </object>\
                  <packing>\
                    <property name=\"left-attach\">1</property>\
                    <property name=\"top-attach\">2</property>\
                  </packing>\
                </child>\
                <child>\
                  <object class=\"GtkButton\" id=\"tanh button\">\
                    <property name=\"visible\">True</property>\
                    <property name=\"can-focus\">True</property>\
                    <property name=\"receives-default\">True</property>\
                    <signal name=\"clicked\" handler=\"ButtonClickedHandler\" object=\"tanh\" swapped=\"no\"/>\
                    <child>\
                      <object class=\"GtkLabel\" id=\"tanh\">\
                        <property name=\"visible\">True</property>\
                        <property name=\"can-focus\">False</property>\
                        <property name=\"label\" translatable=\"yes\">tanh</property>\
                        <attributes>\
                          <attribute name=\"font-desc\" value=\"Segoe UI Bold 15\"/>\
                          <attribute name=\"style\" value=\"oblique\"/>\
                        </attributes>\
                      </object>\
                    </child>\
                  </object>\
                  <packing>\
                    <property name=\"left-attach\">2</property>\
                    <property name=\"top-attach\">2</property>\
                  </packing>\
                </child>\
                <child>\
                  <object class=\"GtkButton\" id=\"asinh button\">\
                    <property name=\"visible\">True</property>\
                    <property name=\"can-focus\">True</property>\
                    <property name=\"receives-default\">True</property>\
                    <signal name=\"clicked\" handler=\"ButtonClickedHandler\" object=\"asinh\" swapped=\"no\"/>\
                    <child>\
                      <object class=\"GtkLabel\" id=\"asinh\">\
                        <property name=\"visible\">True</property>\
                        <property name=\"can-focus\">False</property>\
                        <property name=\"label\" translatable=\"yes\">asinh</property>\
                        <attributes>\
                          <attribute name=\"font-desc\" value=\"Segoe UI Bold 15\"/>\
                          <attribute name=\"style\" value=\"oblique\"/>\
                        </attributes>\
                      </object>\
                    </child>\
                  </object>\
                  <packing>\
                    <property name=\"left-attach\">0</property>\
                    <property name=\"top-attach\">3</property>\
                  </packing>\
                </child>\
                <child>\
                  <object class=\"GtkButton\" id=\"acosh button\">\
                    <property name=\"visible\">True</property>\
                    <property name=\"can-focus\">True</property>\
                    <property name=\"receives-default\">True</property>\
                    <signal name=\"clicked\" handler=\"ButtonClickedHandler\" object=\"acosh\" swapped=\"no\"/>\
                    <child>\
                      <object class=\"GtkLabel\" id=\"acosh\">\
                        <property name=\"visible\">True</property>\
                        <property name=\"can-focus\">False</property>\
                        <property name=\"label\" translatable=\"yes\">acosh</property>\
                        <attributes>\
                          <attribute name=\"font-desc\" value=\"Segoe UI Bold 15\"/>\
                          <attribute name=\"style\" value=\"oblique\"/>\
                        </attributes>\
                      </object>\
                    </child>\
                  </object>\
                  <packing>\
                    <property name=\"left-attach\">1</property>\
                    <property name=\"top-attach\">3</property>\
                  </packing>\
                </child>\
                <child>\
                  <object class=\"GtkButton\" id=\"atanh button\">\
                    <property name=\"visible\">True</property>\
                    <property name=\"can-focus\">True</property>\
                    <property name=\"receives-default\">True</property>\
                    <signal name=\"clicked\" handler=\"ButtonClickedHandler\" object=\"atanh\" swapped=\"no\"/>\
                    <child>\
                      <object class=\"GtkLabel\" id=\"atanh\">\
                        <property name=\"visible\">True</property>\
                        <property name=\"can-focus\">False</property>\
                        <property name=\"label\" translatable=\"yes\">atanh</property>\
                        <attributes>\
                          <attribute name=\"font-desc\" value=\"Segoe UI Bold 15\"/>\
                          <attribute name=\"style\" value=\"oblique\"/>\
                        </attributes>\
                      </object>\
                    </child>\
                  </object>\
                  <packing>\
                    <property name=\"left-attach\">2</property>\
                    <property name=\"top-attach\">3</property>\
                  </packing>\
                </child>\
                <child>\
                  <object class=\"GtkButton\" id=\"cosec button\">\
                    <property name=\"visible\">True</property>\
                    <property name=\"can-focus\">True</property>\
                    <property name=\"receives-default\">True</property>\
                    <signal name=\"clicked\" handler=\"ButtonClickedHandler\" object=\"cosec\" swapped=\"no\"/>\
                    <child>\
                      <object class=\"GtkLabel\" id=\"cosec\">\
                        <property name=\"visible\">True</property>\
                        <property name=\"can-focus\">False</property>\
                        <property name=\"label\" translatable=\"yes\">cosec</property>\
                        <attributes>\
                          <attribute name=\"font-desc\" value=\"Segoe UI Bold 15\"/>\
                          <attribute name=\"style\" value=\"oblique\"/>\
                        </attributes>\
                      </object>\
                    </child>\
                  </object>\
                  <packing>\
                    <property name=\"left-attach\">0</property>\
                    <property name=\"top-attach\">4</property>\
                  </packing>\
                </child>\
                <child>\
                  <object class=\"GtkButton\" id=\"sec button\">\
                    <property name=\"visible\">True</property>\
                    <property name=\"can-focus\">True</property>\
                    <property name=\"receives-default\">True</property>\
                    <signal name=\"clicked\" handler=\"ButtonClickedHandler\" object=\"sec\" swapped=\"no\"/>\
                    <child>\
                      <object class=\"GtkLabel\" id=\"sec\">\
                        <property name=\"visible\">True</property>\
                        <property name=\"can-focus\">False</property>\
                        <property name=\"label\" translatable=\"yes\">sec</property>\
                        <attributes>\
                          <attribute name=\"font-desc\" value=\"Segoe UI Bold 15\"/>\
                          <attribute name=\"style\" value=\"oblique\"/>\
                        </attributes>\
                      </object>\
                    </child>\
                  </object>\
                  <packing>\
                    <property name=\"left-attach\">1</property>\
                    <property name=\"top-attach\">4</property>\
                  </packing>\
                </child>\
                <child>\
                  <object class=\"GtkButton\" id=\"cot button\">\
                    <property name=\"visible\">True</property>\
                    <property name=\"can-focus\">True</property>\
                    <property name=\"receives-default\">True</property>\
                    <signal name=\"clicked\" handler=\"ButtonClickedHandler\" object=\"cot\" swapped=\"no\"/>\
                    <child>\
                      <object class=\"GtkLabel\" id=\"cot\">\
                        <property name=\"visible\">True</property>\
                        <property name=\"can-focus\">False</property>\
                        <property name=\"label\" translatable=\"yes\">cot</property>\
                        <attributes>\
                          <attribute name=\"font-desc\" value=\"Segoe UI Bold 15\"/>\
                          <attribute name=\"style\" value=\"oblique\"/>\
                        </attributes>\
                      </object>\
                    </child>\
                  </object>\
                  <packing>\
                    <property name=\"left-attach\">2</property>\
                    <property name=\"top-attach\">4</property>\
                  </packing>\
                </child>\
                <child>\
                  <object class=\"GtkButton\" id=\"cosech button\">\
                    <property name=\"visible\">True</property>\
                    <property name=\"can-focus\">True</property>\
                    <property name=\"receives-default\">True</property>\
                    <signal name=\"clicked\" handler=\"ButtonClickedHandler\" object=\"cosech\" swapped=\"no\"/>\
                    <child>\
                      <object class=\"GtkLabel\" id=\"cosech\">\
                        <property name=\"visible\">True</property>\
                        <property name=\"can-focus\">False</property>\
                        <property name=\"label\" translatable=\"yes\">cosech</property>\
                        <attributes>\
                          <attribute name=\"font-desc\" value=\"Segoe UI Bold 15\"/>\
                          <attribute name=\"style\" value=\"oblique\"/>\
                        </attributes>\
                      </object>\
                    </child>\
                  </object>\
                  <packing>\
                    <property name=\"left-attach\">0</property>\
                    <property name=\"top-attach\">5</property>\
                  </packing>\
                </child>\
                <child>\
                  <object class=\"GtkButton\" id=\"sech button\">\
                    <property name=\"visible\">True</property>\
                    <property name=\"can-focus\">True</property>\
                    <property name=\"receives-default\">True</property>\
                    <signal name=\"clicked\" handler=\"ButtonClickedHandler\" object=\"sech\" swapped=\"no\"/>\
                    <child>\
                      <object class=\"GtkLabel\" id=\"sech\">\
                        <property name=\"visible\">True</property>\
                        <property name=\"can-focus\">False</property>\
                        <property name=\"label\" translatable=\"yes\">sech</property>\
                        <attributes>\
                          <attribute name=\"font-desc\" value=\"Segoe UI Bold 15\"/>\
                          <attribute name=\"style\" value=\"oblique\"/>\
                        </attributes>\
                      </object>\
                    </child>\
                  </object>\
                  <packing>\
                    <property name=\"left-attach\">1</property>\
                    <property name=\"top-attach\">5</property>\
                  </packing>\
                </child>\
                <child>\
                  <object class=\"GtkButton\" id=\"coth button\">\
                    <property name=\"visible\">True</property>\
                    <property name=\"can-focus\">True</property>\
                    <property name=\"receives-default\">True</property>\
                    <signal name=\"clicked\" handler=\"ButtonClickedHandler\" object=\"coth\" swapped=\"no\"/>\
                    <child>\
                      <object class=\"GtkLabel\" id=\"coth\">\
                        <property name=\"visible\">True</property>\
                        <property name=\"can-focus\">False</property>\
                        <property name=\"label\" translatable=\"yes\">coth</property>\
                        <attributes>\
                          <attribute name=\"font-desc\" value=\"Segoe UI Bold 15\"/>\
                          <attribute name=\"style\" value=\"oblique\"/>\
                        </attributes>\
                      </object>\
                    </child>\
                  </object>\
                  <packing>\
                    <property name=\"left-attach\">2</property>\
                    <property name=\"top-attach\">5</property>\
                  </packing>\
                </child>\
                <child>\
                  <object class=\"GtkButton\" id=\"ln button\">\
                    <property name=\"visible\">True</property>\
                    <property name=\"can-focus\">True</property>\
                    <property name=\"receives-default\">True</property>\
                    <signal name=\"clicked\" handler=\"ButtonClickedHandler\" object=\"ln\" swapped=\"no\"/>\
                    <child>\
                      <object class=\"GtkLabel\" id=\"ln\">\
                        <property name=\"visible\">True</property>\
                        <property name=\"can-focus\">False</property>\
                        <property name=\"label\" translatable=\"yes\">ln</property>\
                        <attributes>\
                          <attribute name=\"font-desc\" value=\"Segoe UI Bold 15\"/>\
                          <attribute name=\"style\" value=\"oblique\"/>\
                        </attributes>\
                      </object>\
                    </child>\
                  </object>\
                  <packing>\
                    <property name=\"left-attach\">1</property>\
                    <property name=\"top-attach\">6</property>\
                  </packing>\
                </child>\
                <child>\
                  <object class=\"GtkButton\" id=\"log button\">\
                    <property name=\"visible\">True</property>\
                    <property name=\"can-focus\">True</property>\
                    <property name=\"receives-default\">True</property>\
                    <signal name=\"clicked\" handler=\"ButtonClickedHandler\" object=\"log\" swapped=\"no\"/>\
                    <child>\
                      <object class=\"GtkLabel\" id=\"log\">\
                        <property name=\"visible\">True</property>\
                        <property name=\"can-focus\">False</property>\
                        <property name=\"label\" translatable=\"yes\">log</property>\
                        <attributes>\
                          <attribute name=\"font-desc\" value=\"Segoe UI Bold 15\"/>\
                          <attribute name=\"style\" value=\"oblique\"/>\
                        </attributes>\
                      </object>\
                    </child>\
                  </object>\
                  <packing>\
                    <property name=\"left-attach\">0</property>\
                    <property name=\"top-attach\">6</property>\
                  </packing>\
                </child>\
                <child>\
                  <object class=\"GtkButton\" id=\"sqrt button\">\
                    <property name=\"visible\">True</property>\
                    <property name=\"can-focus\">True</property>\
                    <property name=\"receives-default\">True</property>\
                    <signal name=\"clicked\" handler=\"ButtonClickedHandler\" object=\"sqrt\" swapped=\"no\"/>\
                    <child>\
                      <object class=\"GtkLabel\" id=\"sqrt\">\
                        <property name=\"visible\">True</property>\
                        <property name=\"can-focus\">False</property>\
                        <property name=\"label\" translatable=\"yes\">sqrt</property>\
                        <attributes>\
                          <attribute name=\"font-desc\" value=\"Segoe UI Bold 15\"/>\
                          <attribute name=\"style\" value=\"oblique\"/>\
                        </attributes>\
                      </object>\
                    </child>\
                  </object>\
                  <packing>\
                    <property name=\"left-attach\">0</property>\
                    <property name=\"top-attach\">7</property>\
                  </packing>\
                </child>\
                <child>\
                  <object class=\"GtkButton\" id=\"cbrt button\">\
                    <property name=\"visible\">True</property>\
                    <property name=\"can-focus\">True</property>\
                    <property name=\"receives-default\">True</property>\
                    <signal name=\"clicked\" handler=\"ButtonClickedHandler\" object=\"cbrt\" swapped=\"no\"/>\
                    <child>\
                      <object class=\"GtkLabel\" id=\"cbrt\">\
                        <property name=\"visible\">True</property>\
                        <property name=\"can-focus\">False</property>\
                        <property name=\"label\" translatable=\"yes\">cbrt</property>\
                        <attributes>\
                          <attribute name=\"font-desc\" value=\"Segoe UI Bold 15\"/>\
                          <attribute name=\"style\" value=\"oblique\"/>\
                        </attributes>\
                      </object>\
                    </child>\
                  </object>\
                  <packing>\
                    <property name=\"left-attach\">1</property>\
                    <property name=\"top-attach\">7</property>\
                  </packing>\
                </child>\
                <child>\
                  <object class=\"GtkButton\" id=\"^ button\">\
                    <property name=\"visible\">True</property>\
                    <property name=\"can-focus\">True</property>\
                    <property name=\"receives-default\">True</property>\
                    <signal name=\"clicked\" handler=\"ButtonClickedHandler\" object=\"^\" swapped=\"no\"/>\
                    <child>\
                      <object class=\"GtkLabel\" id=\"^\">\
                        <property name=\"visible\">True</property>\
                        <property name=\"can-focus\">False</property>\
                        <property name=\"label\" translatable=\"yes\">^</property>\
                        <attributes>\
                          <attribute name=\"font-desc\" value=\"Segoe UI Bold 15\"/>\
                          <attribute name=\"style\" value=\"oblique\"/>\
                        </attributes>\
                      </object>\
                    </child>\
                  </object>\
                  <packing>\
                    <property name=\"left-attach\">2</property>\
                    <property name=\"top-attach\">6</property>\
                  </packing>\
                </child>\
                <child>\
                  <placeholder/>\
                </child>\
              </object>\
              <packing>\
                <property name=\"name\">page1</property>\
                <property name=\"title\" translatable=\"yes\">page1</property>\
                <property name=\"position\">1</property>\
              </packing>\
            </child>\
          </object>\
          <packing>\
            <property name=\"left-attach\">0</property>\
            <property name=\"top-attach\">2</property>\
            <property name=\"width\">4</property>\
            <property name=\"height\">6</property>\
          </packing>\
        </child>\
        <child>\
          <object class=\"GtkButton\" id=\"switch button\">\
            <property name=\"visible\">True</property>\
            <property name=\"can-focus\">True</property>\
            <property name=\"receives-default\">True</property>\
            <signal name=\"clicked\" handler=\"StackSwitchHandler\" object=\"number/function stack\" swapped=\"no\"/>\
            <child>\
              <object class=\"GtkLabel\" id=\"switch\">\
                <property name=\"visible\">True</property>\
                <property name=\"can-focus\">False</property>\
                <property name=\"label\" translatable=\"yes\">f</property>\
                <attributes>\
                  <attribute name=\"font-desc\" value=\"Vivaldi Italic Condensed 25\"/>\
                  <attribute name=\"style\" value=\"oblique\"/>\
                  <attribute name=\"weight\" value=\"ultrabold\"/>\
                </attributes>\
              </object>\
            </child>\
          </object>\
          <packing>\
            <property name=\"left-attach\">4</property>\
            <property name=\"top-attach\">2</property>\
          </packing>\
        </child>\
        <child>\
          <object class=\"GtkButton\" id=\"CE button\">\
            <property name=\"visible\">True</property>\
            <property name=\"can-focus\">True</property>\
            <property name=\"receives-default\">True</property>\
            <signal name=\"clicked\" handler=\"CEhandler\" object=\"expression label\" swapped=\"no\"/>\
            <child>\
              <object class=\"GtkLabel\" id=\"CE\">\
                <property name=\"visible\">True</property>\
                <property name=\"can-focus\">False</property>\
                <property name=\"label\" translatable=\"yes\">CE</property>\
                <attributes>\
                  <attribute name=\"font-desc\" value=\"Segoe UI Bold 15\"/>\
                  <attribute name=\"style\" value=\"oblique\"/>\
                </attributes>\
              </object>\
            </child>\
          </object>\
          <packing>\
            <property name=\"left-attach\">4</property>\
            <property name=\"top-attach\">3</property>\
          </packing>\
        </child>\
        <child>\
          <object class=\"GtkButton\" id=\"+ button\">\
            <property name=\"visible\">True</property>\
            <property name=\"can-focus\">True</property>\
            <property name=\"receives-default\">True</property>\
            <signal name=\"clicked\" handler=\"ButtonClickedHandler\" object=\"+\" swapped=\"no\"/>\
            <child>\
              <object class=\"GtkLabel\" id=\"+\">\
                <property name=\"visible\">True</property>\
                <property name=\"can-focus\">False</property>\
                <property name=\"label\" translatable=\"yes\">+</property>\
                <attributes>\
                  <attribute name=\"font-desc\" value=\"Segoe UI Bold 20\"/>\
                  <attribute name=\"style\" value=\"oblique\"/>\
                </attributes>\
              </object>\
            </child>\
          </object>\
          <packing>\
            <property name=\"left-attach\">4</property>\
            <property name=\"top-attach\">4</property>\
          </packing>\
        </child>\
        <child>\
          <object class=\"GtkButton\" id=\"- button\">\
            <property name=\"visible\">True</property>\
            <property name=\"can-focus\">True</property>\
            <property name=\"receives-default\">True</property>\
            <signal name=\"clicked\" handler=\"ButtonClickedHandler\" object=\"-\" swapped=\"no\"/>\
            <child>\
              <object class=\"GtkLabel\" id=\"-\">\
                <property name=\"visible\">True</property>\
                <property name=\"can-focus\">False</property>\
                <property name=\"label\" translatable=\"yes\">-</property>\
                <attributes>\
                  <attribute name=\"font-desc\" value=\"Segoe UI Bold 25\"/>\
                  <attribute name=\"style\" value=\"oblique\"/>\
                </attributes>\
              </object>\
            </child>\
          </object>\
          <packing>\
            <property name=\"left-attach\">4</property>\
            <property name=\"top-attach\">5</property>\
          </packing>\
        </child>\
        <child>\
          <object class=\"GtkButton\" id=\"* button\">\
            <property name=\"visible\">True</property>\
            <property name=\"can-focus\">True</property>\
            <property name=\"receives-default\">True</property>\
            <signal name=\"clicked\" handler=\"ButtonClickedHandler\" object=\"*\" swapped=\"no\"/>\
            <child>\
              <object class=\"GtkLabel\" id=\"*\">\
                <property name=\"visible\">True</property>\
                <property name=\"can-focus\">False</property>\
                <property name=\"label\" translatable=\"yes\">*</property>\
                <attributes>\
                  <attribute name=\"font-desc\" value=\"Segoe UI Bold 20\"/>\
                  <attribute name=\"style\" value=\"oblique\"/>\
                </attributes>\
              </object>\
            </child>\
          </object>\
          <packing>\
            <property name=\"left-attach\">4</property>\
            <property name=\"top-attach\">6</property>\
          </packing>\
        </child>\
        <child>\
          <object class=\"GtkButton\" id=\"/ button\">\
            <property name=\"visible\">True</property>\
            <property name=\"can-focus\">True</property>\
            <property name=\"receives-default\">True</property>\
            <signal name=\"clicked\" handler=\"ButtonClickedHandler\" object=\"/\" swapped=\"no\"/>\
            <child>\
              <object class=\"GtkLabel\" id=\"/\">\
                <property name=\"visible\">True</property>\
                <property name=\"can-focus\">False</property>\
                <property name=\"label\" translatable=\"yes\">/</property>\
                <attributes>\
                  <attribute name=\"font-desc\" value=\"Segoe UI Bold 20\"/>\
                  <attribute name=\"style\" value=\"oblique\"/>\
                </attributes>\
              </object>\
            </child>\
          </object>\
          <packing>\
            <property name=\"left-attach\">4</property>\
            <property name=\"top-attach\">7</property>\
          </packing>\
        </child>\
        <child>\
          <object class=\"GtkFrame\">\
            <property name=\"visible\">True</property>\
            <property name=\"can-focus\">False</property>\
            <property name=\"label-xalign\">0</property>\
            <property name=\"shadow-type\">none</property>\
            <child>\
              <object class=\"GtkAlignment\">\
                <property name=\"visible\">True</property>\
                <property name=\"can-focus\">False</property>\
                <child>\
                  <object class=\"GtkBox\">\
                    <property name=\"visible\">True</property>\
                    <property name=\"can-focus\">False</property>\
                    <property name=\"orientation\">vertical</property>\
                    <property name=\"baseline-position\">bottom</property>\
                    <child>\
                      <object class=\"GtkLabel\" id=\"result label\">\
                        <property name=\"visible\">True</property>\
                        <property name=\"can-focus\">False</property>\
                        <property name=\"label\" translatable=\"yes\">result</property>\
                        <property name=\"justify\">right</property>\
                        <property name=\"xalign\">0.9800000190734863</property>\
                        <attributes>\
                          <attribute name=\"font-desc\" value=\"Sans Bold Oblique 10\"/>\
                          <attribute name=\"foreground\" value=\"#9a9a99999696\"/>\
                        </attributes>\
                      </object>\
                      <packing>\
                        <property name=\"expand\">False</property>\
                        <property name=\"fill\">True</property>\
                        <property name=\"position\">0</property>\
                      </packing>\
                    </child>\
                    <child>\
                      <object class=\"GtkScrolledWindow\">\
                        <property name=\"visible\">True</property>\
                        <property name=\"can-focus\">True</property>\
                        <property name=\"shadow-type\">in</property>\
                        <child>\
                          <object class=\"GtkViewport\">\
                            <property name=\"visible\">True</property>\
                            <property name=\"can-focus\">False</property>\
                            <child>\
                              <object class=\"GtkLabel\" id=\"expression label\">\
                                <property name=\"visible\">True</property>\
                                <property name=\"can-focus\">False</property>\
                                <property name=\"label\" translatable=\"yes\">expression</property>\
                                <property name=\"justify\">right</property>\
                                <property name=\"xalign\">0.9800000190734863</property>\
                                <attributes>\
                                  <attribute name=\"font-desc\" value=\"Sans Bold 12\"/>\
                                </attributes>\
                                <signal name=\"notify::label\" handler=\"calculateHandler\" object=\"result label\" swapped=\"no\"/>\
                              </object>\
                            </child>\
                          </object>\
                        </child>\
                      </object>\
                      <packing>\
                        <property name=\"expand\">True</property>\
                        <property name=\"fill\">True</property>\
                        <property name=\"position\">1</property>\
                      </packing>\
                    </child>\
                  </object>\
                </child>\
              </object>\
            </child>\
            <child type=\"label\">\
              <object class=\"GtkLabel\" id=\"prototype label\">\
                <property name=\"visible\">True</property>\
                <property name=\"can-focus\">False</property>\
                <property name=\"label\" translatable=\"yes\">Prototype</property>\
                <attributes>\
                  <attribute name=\"font-desc\" value=\"Segoe UI Semi-Bold Italic 10\"/>\
                  <attribute name=\"style\" value=\"italic\"/>\
                </attributes>\
              </object>\
            </child>\
          </object>\
          <packing>\
            <property name=\"left-attach\">0</property>\
            <property name=\"top-attach\">0</property>\
            <property name=\"width\">5</property>\
            <property name=\"height\">2</property>\
          </packing>\
        </child>\
      </object>\
    </child>\
  </object>\
</interface>\
";
	
	GError *error = NULL;
	
	//We create the interface through the string
	if(!gtk_builder_add_from_string(builder, interface, -1, &error)){
		g_printerr("Error: %s\n",error->message);
		g_clear_error(&error);
		g_object_unref(builder);
		return 1;
	}
	
	expression = GTK_WIDGET(gtk_builder_get_object(builder, "expression label"));
	
	window = GTK_WIDGET(gtk_builder_get_object(builder, "mainWindow"));
	
	gtk_builder_connect_signals(builder, NULL);
	
	gtk_widget_show_all(window);
	gtk_main();
	
	g_object_unref(builder);
	return 0;
}
