#ifndef UTILITY_TEMPLATE_HPP_
#define UTILITY_TEMPLATE_HPP_

template<typename C>
class singleton {
public:
	static C* instance() {
		if (!_instance)
			_instance = new C();
		return _instance;
	}
	virtual ~singleton() {
		_instance = 0;
	}
private:
	static C* _instance;
protected:
	singleton() {
	}
};
template<typename C> C* singleton<C>::_instance = 0;


#endif /* UTILITY_TEMPLATE_HPP_ */
