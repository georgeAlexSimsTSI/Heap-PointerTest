
#include <iostream>
#include <string>
#include <memory>
using std::cout;
using std::endl;
using std::string;

class Object
{
private:
    string message;

public:
    explicit Object(const string &message) : message(message) { cout << "Object with message " << message << " is being constructed" << endl; }
    Object() : Object("") { cout << "Object empty constructor is called " << endl; };
    ~Object() { cout << "Object with message: " << message << " Is being destructed" << endl; }
    void use() const { cout << "Object is being used: " << message << endl; }
    string getMessage() const { return message; }
    void setMessage(const string &message) { this->message = message; }
};

int main()
{
    {
        Object obj = Object();
    }

    Object *objPointer = new Object("Hello World");
    delete objPointer;

    // unique pointer https://iamsorush.com/posts/unique-pointers-cpp/
    {
        std::unique_ptr<Object> uniquePtr = std::make_unique<Object>("Unique Ptr"); // will delete for us when the pointer goes out of scope
    }
    std::unique_ptr<Object> uniquePtr2;
    {
        std::unique_ptr<Object> uniquePtr(new Object("Unique Ptr")); // will delete for us when the pointer goes out of scope
        if (uniquePtr.get() != nullptr)
            cout << "Unique pointer holds an object" << endl;
        uniquePtr2 = move(uniquePtr);
        uniquePtr2->setMessage("Unique pointer 2");
        if (uniquePtr.get() == nullptr)
            cout << "Unique pointer no longer holds any object " << endl;
    }
    cout << endl
         << endl;

    // shared pointers https://iamsorush.com/posts/shared-pointer-cpp/
    std::shared_ptr<Object> sharedPtr1;
    {
        std::shared_ptr<Object> sharedPtr2 = std::make_shared<Object>("Shared Ptr");
        if (sharedPtr2.get() != nullptr)
            cout << "shared pointer holds an object" << endl;
        else
            cout << "shared pointer doesn't hold an object " << endl;

        cout << "Object is used by " << sharedPtr2.use_count() << endl;
        sharedPtr1 = sharedPtr2;
        if (sharedPtr2.get() != nullptr)
            cout << "shared pointer holds an object" << endl;
        cout << "Object is used by " << sharedPtr2.use_count() << endl;
        cout << "Object is unique: " << sharedPtr2.unique() << endl;
    }
    cout << "Object is used by " << sharedPtr1.use_count() << endl;
    cout << "Object is unique: " << sharedPtr1.unique() << endl;

    cout << endl
         << endl;
    // Weak pointers
    // a weak pointer is used to observe the object of a shared pointer https://iamsorush.com/posts/weak-pointer-cpp/

    std::weak_ptr<Object> wp = sharedPtr1;
    if (auto tmp = wp.lock())
        cout << tmp->getMessage() << endl;
    std::shared_ptr<Object> sharedPtr2;
    sharedPtr2.reset();
    sharedPtr1.reset(new Object("Weak Ptr Test"));
    if (wp.expired())
        cout << "The shared pointer has expired " << endl; // object isn't destructed until the weak pointers are also destructed
}