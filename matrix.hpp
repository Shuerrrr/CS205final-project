#ifndef _MATRIX_
#define _MATRIX_

#include <iostream>
using namespace std;

struct Rectangle{
    size_t row,col,width,height;
};

Rectangle Rect(size_t row,size_t col,size_t width,size_t height);

template<class T,size_t channel>
class matrix
{
private:
    size_t rows,cols;
    T *data;
    size_t *refcount;
    size_t channels=channel;
    size_t step;
public:

    bool isvalid=false;

    matrix(size_t rows,size_t cols,const T *data):rows(rows),cols(cols),step(cols*channel)
    {
        if(rows>0&&cols>0&&this->channels>0){
        this->refcount=(size_t *)(new char[rows*cols*channels*sizeof(T)+sizeof(size_t)]{});
        this->data=(T *)((char *)this->refcount+sizeof(size_t));
        for(size_t i=0;i<rows*cols*channels;i++)
        {
            this->data[i]=data[i];
        }
        this->refcount[0]=1;
        this->isvalid=true;
        }
        //...
        else
        {
            this->isvalid=false;
            this->data=0;
            cout<<"invalid matrix."<<endl;
        }
    }

    matrix(const matrix& mat);

    matrix(const matrix& mat,const Rectangle& rect);

    size_t get_channels(){return channels;}

    void print_matrix();

    bool operator=(const matrix& mat);

    matrix operator+(const matrix& mat);

    template<class CT,size_t channel_,typename Q>
    friend matrix<CT,channel_> operator+(Q num,const matrix<CT,channel_>& mat);

    template<class CT,size_t channel_,typename Q>
    friend matrix<CT,channel_> operator+(const matrix<CT,channel_>& mat,Q num);

    bool operator==(const matrix& mat);

    matrix operator-(const matrix& mat);

    template<typename Q>
    matrix operator-(Q num);

    matrix operator*(const matrix& mat);

    template<class CT,size_t channel_,typename Q>
    friend matrix<CT,channel_> operator*(Q num,const matrix<CT,channel_>& mat);

    template<class CT,size_t channel_,typename Q>
    friend matrix<CT,channel_> operator*(const matrix<CT,channel_>& mat,Q num);

    bool clone(const matrix& mat);

    ~matrix(){
        cout<<"des~ has been run :"<<this->data[0]<<endl;
        if(this->refcount[0]<=1){
        delete[] this->refcount;
        cout<<"data has been deleted."<<endl;
        }
        else{
        this->refcount[0]--;
        cout<<"data is still alive :"<<this->refcount[0]<<endl;
        }
    }
};

template<class T,size_t channel>
bool matrix<T,channel>::operator==(const matrix<T,channel>& mat)
{
    if((!mat.data)||this->isvalid==false||mat.isvalid==false){
        return false;
    }
    else if(mat.rows!=this->rows||mat.cols!=this->cols){
        return false;
    }
    else{
        T *p1=this->data;
        T *p2=mat.data;
        for(size_t i=0;i<this->rows;i++){
            for(size_t j=0;j<this->cols*this->channels;j++){
                if(p1[j]!=p2[j])
                return false;
            }
            p2+=mat.step;
            p1+=this->step;
        }
    }
    return true;
}


template<class T,size_t channel>
matrix<T,channel>::matrix(const matrix& mat)
{
    if((!mat.data)||mat.isvalid==false){
        cout<<"null data"<<endl;
        this->isvalid=false;
    }
    else{
    this->cols=mat.cols;
    this->rows=mat.rows;
    this->channels=mat.channels;
    this->refcount=mat.refcount;
    this->data=mat.data;
    this->refcount[0]++;
    this->step=mat.step;
    cout<<"copy is used"<<endl;
    this->isvalid=true;
    //...
    }
}

template<class T,size_t channel>
matrix<T,channel>::matrix(const matrix& mat,const Rectangle& rect)
{
    if((!mat.data)||mat.isvalid==false){
        cout<<"null data"<<endl;
        this->isvalid=false;
    }
    else if(rect.row+rect.height>mat.rows||rect.col*+rect.width>mat.cols)
    {
        cout<<"submatrix is out of range"<<endl;
        this->isvalid=false;
    }
    else{
    this->rows=rect.height;
    this->cols=rect.width;
    this->channels=channel;
    this->refcount=mat.refcount;
    this->data=mat.data+rect.col*channel+rect.row*mat.step;
    this->step=mat.step;
    this->refcount[0]++;
    cout<<"ROI is used"<<endl;
    this->isvalid=true;
    }
}

template<class T,size_t channel>
bool matrix<T,channel>::operator=(const matrix<T,channel>& mat)
{
    if((!mat.data)||mat.isvalid==false){
        cout<<"null data"<<endl;
        this->isvalid=false;
        return false;
    }
    if(!this->refcount)
    delete []this->refcount;
    
    this->cols=mat.cols;
    this->rows=mat.rows;
    this->channels=mat.channels;
    this->refcount=mat.refcount;
    this->data=mat.data;
    this->refcount[0]++;
    this->step=mat.step;
    cout<<"= is used"<<endl;
    this->isvalid=true;
    return true;
}

template<class T,size_t channel>
matrix<T,channel> matrix<T,channel>::operator+(const matrix<T,channel>& mat)
{
    matrix<T,channel> matr(this->rows,this->cols,this->data);
    if(this->cols==mat.cols&&this->rows==mat.rows&&this->channels==mat.channels&&this->data!=0&&mat.data!=0&&this->isvalid&&mat.isvalid)
    {
        // for(int i=0;i<this->rows*this->cols*this->channels;i++){
        //     matr.data[i]+=mat.data[i];
        // }
        T *p1=this->data;
        T *p2=mat.data;
        T *result=matr.data;
    for(size_t i=0;i<this->rows;i++){
        for(size_t j=0;j<this->cols*this->channels;j++){
            result[j]=p1[j]+p2[j];
        }
        result+=matr.step;
        p1+=this->step;
        p2+=mat.step;
    }
        matr.isvalid=true;
        return matr;
    }
    else
    {
        cout<<"operation + invalid"<<endl;
        return matr;
    }
}

template<class T,size_t channel,typename Q>
matrix<T,channel> operator+(Q num,const matrix<T,channel>& mat)
{
    if((!mat.data)||mat.isvalid==false){
        cout<<"null data"<<endl;
        return mat;
    }
    matrix<T,channel> resultmat(mat.rows,mat.cols,mat.data);
    // for(int i=0;i<mat.rows*mat.cols*mat.channels;i++){
    //     resultmat.data[i]+=(T)num;
    // }
    T *p=mat.data;
    T* result=resultmat.data;
    for(size_t i=0;i<resultmat.rows;i++){
        for(size_t j=0;j<resultmat.cols*resultmat.channels;j++){
            result[j]=p[j]+(T)num;
        }
        p+=mat.step;
        result+=resultmat.step;
    }
    resultmat.isvalid=true;
    return resultmat;
}

template<class T,size_t channel,typename Q>
matrix<T,channel> operator+(const matrix<T,channel>& mat,Q num)
{
    if((!mat.data)||mat.isvalid==false){
        cout<<"null data"<<endl;
        return mat;
    }
    matrix<T,channel> resultmat(mat.rows,mat.cols,mat.data);
    // for(int i=0;i<mat.rows*mat.cols*mat.channels;i++){
    //     resultmat.data[i]+=(T)num;
    // }
    T *p=mat.data;
    T* result=resultmat.data;
    for(size_t i=0;i<resultmat.rows;i++){
        for(size_t j=0;j<resultmat.cols*resultmat.channels;j++){
            result[j]=p[j]+(T)num;
        }
        p+=mat.step;
        result+=resultmat.step;
    }
    cout<<"+ is used"<<endl;
    resultmat.isvalid=true;
    return resultmat;
}

template<class T,size_t channel>
matrix<T,channel> matrix<T,channel>::operator-(const matrix<T,channel>& mat)
{
    matrix<T,channel> matr(this->rows,this->cols,this->data);
    if(this->cols==mat.cols&&this->rows==mat.rows&&this->channels==mat.channels&&this->data!=0&&mat.data!=0&&this->isvalid&&mat.isvalid)
    {
        // for(int i=0;i<this->rows*this->cols*this->channels;i++){
        //     matr.data[i]-=mat.data[i];
        // }
        T *p1=this->data;
        T *p2=mat.data;
        T *result=matr.data;
    for(size_t i=0;i<this->rows;i++){
        for(size_t j=0;j<this->cols*this->channels;j++){
            result[j]=p1[j]-p2[j];
        }
        result+=matr.step;
        p1+=this->step;
        p2+=mat.step;
    }
        matr.isvalid=true;
        return matr;
    }
    else
    {
        cout<<"operation - invalid"<<endl;
        matr.isvalid=false;
        return matr;
    }
}

template<class T,size_t channel> template<typename Q>
matrix<T,channel> matrix<T,channel>::operator-(Q num)
{
    if((!this->data)||this->isvalid==false){
        cout<<"null data"<<endl;
        return *this;
    }
    matrix<T,channel> resultmat(this->rows,this->cols,this->data);
    // for(int i=0;i<this->rows*this->cols*this->channels;i++){
    //     resultmat.data[i]-=(T)num;
    // }
        T *p1=this->data;
        T *result=resultmat.data;
    for(size_t i=0;i<this->rows;i++){
        for(size_t j=0;j<this->cols*this->channels;j++){
            result[j]=p1[j]-(T)num;
        }
        result+=resultmat.step;
        p1+=this->step;
    }
    cout<<"- is used"<<endl;
    resultmat.isvalid=true;
    return resultmat;
}

template<class T,size_t channel>
matrix<T,channel> matrix<T,channel>::operator*(const matrix<T,channel>& mat)
{
    if(this->cols==mat.rows&&this->channels==mat.channels&&this->data!=0&&mat.data!=0&&this->isvalid&&mat.isvalid)
    {
        T temp;
        T data[this->rows*mat.cols*channel]={(T)0};

        for(int i=0;i<this->rows;i++){
            for(int t=0;t<channel;t++){
            for(int k=t;k<this->cols*channel;k+=channel){
                temp=this->data[i*this->step+k];
                for(int j=t;j<mat.cols*channel;j+=channel){
                    data[i*this->cols*this->channels+j]+=temp*mat.data[(k/channel)*mat.step+j];
                }
            }
        }
        }
        matrix<T,channel> matr(this->rows,mat.cols,data);
        matr.isvalid=true;
        return matr;
    }
    else
    {
        cout<<"operation * invalid"<<endl;
        return *this;
    }
}


template<class T,size_t channel,typename Q>
matrix<T,channel> operator*(Q num,const matrix<T,channel>& mat)
{
    if((!mat.data)||mat.isvalid=false){
        cout<<"null data"<<endl;
        return mat;
    }
    matrix<T,channel> resultmat(mat.rows,mat.cols,mat.data);
    // for(int i=0;i<mat.rows*mat.cols*mat.channels;i++){
    //     resultmat.data[i]*=(T)num;
    // }
    T *p=mat.data;
    T* result=resultmat.data;
    for(size_t i=0;i<resultmat.rows;i++){
        for(size_t j=0;j<resultmat.cols*resultmat.channels;j++){
            result[j]=p[j]*(T)num;
        }
        p+=mat.step;
        result+=resultmat.step;
    }
    resultmat.isvalid=true;
    return resultmat;
}

template<class T,size_t channel,typename Q>
matrix<T,channel> operator*(const matrix<T,channel>& mat,Q num)
{
    if((!mat.data)||mat.isvalid==false){
        cout<<"null data"<<endl;
        return mat;
    }
    matrix<T,channel> resultmat(mat.rows,mat.cols,mat.data);
    // for(int i=0;i<mat.rows*mat.cols*mat.channels;i++){
    //     resultmat.data[i]*=(T)num;
    // }
    T *p=mat.data;
    T* result=resultmat.data;
    for(size_t i=0;i<resultmat.rows;i++){
        for(size_t j=0;j<resultmat.cols*resultmat.channels;j++){
            result[j]=p[j]*(T)num;
        }
        p+=mat.step;
        result+=resultmat.step;
    }
    cout<<"* is used"<<endl;
    resultmat.isvalid=true;
    return resultmat;
}

template<class T,size_t channel>
void matrix<T,channel>::print_matrix()
{
    if((!this->data)||this->isvalid==false)
    {
        cout<<"null data"<<endl;
        return;
    }
    T *p=this->data;
    for(size_t i=0;i<this->rows;i++){
        for(size_t j=0;j<this->cols*this->channels;j++){
        if(j%this->channels==0){
            cout<<"] [";
        }
        cout<<p[j]<<" ";
        }
        cout<<endl;
        p+=this->step;
    }

    cout<<endl;
}

template<class T,size_t channel>
bool matrix<T,channel>::clone(const matrix<T,channel>& mat)
{
    if((!mat.data)||mat.isvalid==false){
        return false;
    }
    else{
        matrix<T,channel> resultmat(mat.rows,mat.cols,mat.data);
        T *p=mat.data;
        T* result=resultmat.data;
        for(size_t i=0;i<resultmat.rows;i++){
            for(size_t j=0;j<resultmat.cols*resultmat.channels;j++){
                result[j]=p[j];
            }
            p+=mat.step;
            result+=resultmat.step;
        }
        resultmat.isvalid=true;
        *this=resultmat;
        return true;
    }
}

Rectangle Rect(size_t row,size_t col,size_t width,size_t height)
{
    Rectangle ref;
    ref.row=row;
    ref.col=col;
    ref.width=width;
    ref.height=height;
    return ref;
}

#endif
