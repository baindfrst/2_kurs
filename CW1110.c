#include <stdio.h>
#include <stdlib.h>

typedef struct tnode {
    int key;
    struct tnode* left, *right;
} tree;

tree * writeto (FILE * ffrom, tree * root);
tree * Insert(tree *root, int x);
void print_tree(tree* from);
void free_tree (tree * root);

int main(int argc, char **argv)
{
    int i;
    FILE* ffrom;
    tree * root;
    root = NULL;
    for (i =1; i <argc; i++)
    {
        ffrom = fopen (argv[i],"r");
        //printf ("tut\n");
        root = writeto (ffrom, root);
        fclose (ffrom);
    }
    //printf ("tut2\n");

    print_tree (root);
    printf ("\n");
    free_tree(root);
    free (root);
}

tree* writeto (FILE * ffrom, tree * root)
{
    int c;
    while (fscanf(ffrom, "%d", &c) == 1)
    {
        root = Insert(root, c);
    }
    return root;
}

tree * Insert(tree *root, int x)
{
    if (root==NULL)
    {
        root = malloc(sizeof(tree));
        root->key = x;
        root->left = NULL;
        root->right = NULL;
    }
    else
    {
        if (x < root->key)
            root->left = Insert(root->left, x);
        else
            root->right = Insert(root->right, x);

    }
    return root;
}

void print_tree(tree* root)
{
    if (root)
    { //���� �� ���������� ������ ����
        print_tree(root->left); //����������� ������� ��� ������ ���������
        printf ("%d ", root->key);//���������� ������ ������
        print_tree(root->right); //����������� ������� ��� ������� ���������
    }
}

void free_tree (tree * root)
{
    if (root)
    {
        free_tree(root->right);
        free_tree(root->left);
        free(root);
    }
}
