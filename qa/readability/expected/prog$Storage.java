public class prog$Storage {
    public int num1;
    public int num2;
    public int num3;
    public String str1;
    public String str2;
    public String str3;

    public prog$Storage() {
        init();
    }

    public prog$Storage init() {
        num1 = 12345;
        num2 = 9876543;
        num3 = 456;
        str1 = "hello";
        str2 = "world";
        str3 = "abc";
        return this;
    }

    public prog$Storage clear() {
        num1 = 0;
        num2 = 0;
        num3 = 0;
        str1 = "";
        str2 = "";
        str3 = "";
        return this;
    }
}
