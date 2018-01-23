namespace
{
    template<typename T, typename P>
    P point(T& var);

    /// Short description.
    ///
    /// Further details here.
    ///
    /// @param obj  A reference to the object to make a pointer to.
    /// @return     A pointer to the object.
    ///
    /// This function does not fail.
    template<typename T, typename P = T*>
    P point(T& obj)
    {
        return &obj;
    }
}

int main()
{
    int a = 0;
    int* p = point(a);
    (void) p;
}
