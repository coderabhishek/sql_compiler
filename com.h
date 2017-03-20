/* From 'com.c': */
typedef struct node{ 
          int cptr; 
          struct node *child[50]; 
          char s[50]; 
  } node; 
   
node *create(char *s){ 
        node *nd = (node *)malloc(sizeof(node)); 
        strcpy(nd->s, s); 
        nd->cptr = 0; 
        return nd; 
 }
node * match (char *s , int i );
//#include "c-extensions.h"
node * updation_list();
node * COL  (void);
node * identifier  (void);
node * COMMA  (void);
node * BO  (void);
node * BC  (void);
node * WILDCARD  (void);
node * id_list  (void);
node * tab_list  (void);
node * columns  (void);
node * query  (void);
node * sel  (void);
node * selection  (void);
node * selection1  (void);
node * selection2  (void);
node * selection3  (void);
node * selection4  (void);
node * drop  (void);
short int line  (void);
node * CONJUNCTION  (void);
node * COMPARATOR  (void);
node * condition  (void);
node * constraint  (void);
