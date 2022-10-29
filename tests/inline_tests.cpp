#include <iostream>


template<class T> void INLINE_TESTER_SharedPtr_Weak(std::ostream&);
template<class T> void INLINE_TESTER_SharedPtr_Destructor(std::ostream&);
template<class T> void INLINE_TESTER_DeleteChain_Test(std::ostream&);
template<class T> void INLINE_TESTER_SharedPtr_useCount(std::ostream&);
template<class T> void INLINE_TESTER_SharedPtr_BaseToDerived(std::ostream&);
template<class T> void INLINE_TESTER_SharedPtr_Constructors(std::ostream&);


int main() {
  INLINE_TESTER_SharedPtr_Weak<int>(std::cout);
  INLINE_TESTER_SharedPtr_Destructor<int>(std::cout);
  INLINE_TESTER_DeleteChain_Test<int>(std::cout);
  INLINE_TESTER_SharedPtr_useCount<int>(std::cout);
  INLINE_TESTER_SharedPtr_BaseToDerived<int>(std::cout);
  INLINE_TESTER_SharedPtr_Constructors<int>(std::cout);

  std::cout << "All tests passed." << std::endl;

  return 0;
}