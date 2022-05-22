#pragma include_only_once

#define NULL 0
#define topalign (sizeof(struct {unsigned int s; double d;}) - sizeof(double))

typedef void CTOR(void *);
typedef void *ALLOC(unsigned int);
typedef void DTOR(void *, int);
typedef void DEALLOC(void *);

extern "C" void *malloc(int);
extern "C" void  free(void*);
extern void* operator new(unsigned int);
extern void* operator new[](unsigned int);
extern void* operator new(unsigned int,void*);
extern void* operator new[](unsigned int,void*);
extern void  operator delete(void*);
extern void  operator delete[](void*);
extern void *__vec_ctor(unsigned int, unsigned int, bool, CTOR *, ALLOC *);
extern void __vec_dtor(void *, unsigned int, DTOR *, DEALLOC *);


void*
operator new(unsigned int n_)
{
  return (void*)malloc(n_);
}

void*
operator new[](unsigned int n_)
{
  return (void*)malloc(n_);
}

void*
operator new(unsigned int  n_,
             void         *ptr_)
{
  (void)n_;
  return ptr_;
}

void*
operator new[](unsigned int  n_,
               void         *ptr_)
{
  (void)n_;
  return ptr_;
}

// The ARM C++ compiler doesn't use default destructors so it's not
// possible to use obj->~OBJ(); to do "placement delete". The only way
// is to call 'delete'. The problem is then it would free the
// memory. So 'delete' has been changed not to free the memory and
// used for placement delete. SFINAE can't be used to specialize a
// generic destructor/destroy function due to the templating
// implementation.
void
operator delete(void *ptr_)
{
  (void)ptr_;
}

void
operator delete[](void *ptr_)
{
  free(ptr_);
}

void*
__vec_ctor(unsigned int  nelts,
           unsigned int  size,
           bool          save_elts,
           CTOR         *ctor,
           ALLOC        *alloc)
{
  unsigned int alloc_size = nelts * size + (save_elts ? topalign : 0);
  void *p = (alloc != NULL ? alloc : ::operator new[])(alloc_size);

  if(p != NULL)
    {
      if(save_elts)
        {
          *((unsigned int *)p) = nelts;
          p = (char *)p + topalign;
        }

      if(ctor != NULL)
        {
          char *base = (char *)p;
          while (0 < nelts--)
            {
              ctor(base);
              base += size;
            }
        }
    }

  return p;
}

void
__vec_dtor(void         *p,
           unsigned int  size,
           DTOR         *dtor,
           DEALLOC      *dealloc)
{
  if(p != NULL)
    {
      char *q = (char *)p - topalign;
      if(dtor != NULL)
        {
          unsigned int nelts = *((unsigned int*)q);

          char *base = (char *)p + nelts * size;
          while (0 < nelts--)
            {
              base -= size;
              dtor(base, 0);
            }
        }

      (dealloc != NULL ? dealloc : ::operator delete[])(q);
    }
}
