#ifndef KU_MU_MACRO_HPP
#define KU_MU_MACRO_HPP


#define MU_STRINGIZE2(x) #x
#define MU_STRINGIZE(x) MU_STRINGIZE2(x)

// MU_FILE_LINE --- Simplifies usage of __FILE__ and __LINE__
#define MU_FILE_LINE __FILE__ ":" MU_STRINGIZE(__LINE__) ""

#define MU_FUNC __func__




#endif // KU_MU_MACRO_HPP
