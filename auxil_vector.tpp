
template <class T>
void Print(const std::vector<T> &v, const std::string &sep = " ") {
    for (int i : v) {
        std::cout << i << sep;
    }
    std::cout << '\n';
}