#include <iostream>
#include <assert.h>
using namespace std;
struct colNode{
    int data,col;
    colNode*next{};
    colNode*prv{};
    colNode(int data,int col):data(data),col(col){}
};
struct SparseCol{
private:
    colNode*head{},*tail{};
    int columns,len=0;
    void link(colNode*first,colNode*second)
    {
        if (first)
            first->next=second;
        if (second)
            second->prv=first;
    }
    colNode*getidx(int pos,bool missing)
    {
        colNode*top=head;
        while(top->next and top->next->col<pos)
            top=top->next;
        bool check=top->next and (top->next->col==pos);
        if (check)
            return top->next;
        if (missing)
            return nullptr;
        return add(top,0,pos);
    }
    colNode* add(colNode*prv,int data,int pos)
    {
        colNode*mid=new colNode(data,pos);
        len++;
        if (prv==tail)
        {
            link(tail,mid);
            tail=mid;
        }
        else
        {
            link(mid,prv->next);
            link(prv,mid);
        }
        return mid;
    }
public:
    SparseCol(int columns):columns(columns){
        head=tail=new colNode(0,-1);
        len++;
    }
    void set_value(int value,int pos)
    {
        getidx(pos,0)->data=value;
    }
    int get_value(int pos)
    {
        colNode*ans= getidx(pos,1);
        if (!ans)
            return 0;
        return ans->data;
    }
    void print_with_zeros()
    {
        colNode*top=head->next;
        for(int i=0;i<columns;i++)
        {
            if (top and top->col==i)
            {
                cout<<top->data<<' ';
                top=top->next;
            }
            else
                cout<<"0 ";
        }
    cout<<'\n';
    }
    void print()
    {
        for(colNode*top=head->next;top;top=top->next)
            cout<<top->data<<' ';
        cout<<'\n';
    }
    void addcols(SparseCol&other)
    {
        assert(other.columns==columns);
        for(colNode*top=other.head->next;top;top=top->next)
        {
            getidx(top->col,0)->data+=top->data;
        }
    }
};
struct rownode{
    int row{};
    rownode*nxt{};
    rownode*prv{};
    SparseCol cols;
    rownode(int row,int col):row(row),cols(col){}
};
struct SparseMatrix{
private:
    rownode*head{},*tail{};
    int rows=0;
    int cols=0;
    int len=0;
    void link(rownode*first,rownode*second)
    {
        if (first)
            first->nxt=second;
        if (second)
            second->prv=first;
    }
    rownode*get_row(int pos,bool is_missing)
    {
        rownode*top=head;
        while(top->nxt and top->nxt->row<pos)
            top=top->nxt;
        bool check=top->nxt and (top->nxt->row==pos);
        if (check)
            return top->nxt;
        if (is_missing)
            return nullptr;
        return addrow(top,pos);
    }
    rownode*addrow(rownode*other,int pos)
    {
        rownode*mid=new rownode(pos,cols);
        if (other==tail)
        {
            link(other,mid);
            tail=mid;
        }
        else
        {
            link(mid,other->nxt);
            link(other,mid);
        }
        len++;
        return mid;
    }
public:
    SparseMatrix(int lenrow,int lencols):rows(lenrow),cols(lencols){
        tail=head=new rownode(-1,lencols);
        len++;
    }
    void set_value(int value,int idxi,int idxj)
    {
        rownode*cur= get_row(idxi,0);
        cur->cols.set_value(value,idxj);
    }
    int get_value(int idxi,int idxj)
    {
        rownode*cur=get_row(idxi,1);
        return cur->cols.get_value(idxj);
    }
    void print_non_zero()
    {
        for(rownode*top=head->nxt;top;top=top->nxt)
        {
            top->cols.print();
        }
    }
    void print()
    {
        rownode*cur=head->nxt;
        for(int l=0;l<rows;l++)
        {
            if (cur and cur->row==l)
            {
                cur->cols.print_with_zeros();
                cur=cur->nxt;
            }
            else
            {
                for(int j=0;j<cols;j++)
                    cout<<"0 ";
                cout<<'\n';
            }
        }
    }
    void addanotherMatrix(SparseMatrix&other)
    {
        assert(other.rows==rows and other.cols==cols);
        for(rownode*top=other.head->nxt;top;top=top->nxt)
        {
            rownode*idx= get_row(top->row,0);
            idx->cols.addcols(top->cols);
        }
    }
};
int main()
{
    SparseMatrix mat(10,10);
    mat.set_value(3,1,2);
    mat.set_value(4,1,5);
    mat.set_value(5,2,2);
    SparseMatrix mat2(10,10);
    mat2.set_value(3,1,2);
    mat2.set_value(4,3,5);
    mat2.set_value(5,2,2);
    mat.addanotherMatrix(mat2);
    mat.print_non_zero();

    cout<<"NO RTE\n";
}
