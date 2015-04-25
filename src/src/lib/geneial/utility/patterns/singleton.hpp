#ifndef __GENEIAL_SINGLETON_HPP_
#define __GENEIAL_SINGLETON_HPP_

template<typename C>
class singleton
{
public:
    static C* instance()
    {
        if (!_instance)
            _instance = new C();
        return _instance;
    }
    virtual ~singleton()
    {
        delete _instance;
        _instance = 0;
    }
private:
    static C* _instance;
protected:
    singleton()
    {
    }
};
template<typename C> C* singleton<C>::_instance = 0;

#endif /* __GENEIAL_SINGLETON_HPP_ */
