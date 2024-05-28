public class prog {

    public int num1;
    public int num2;
    public int num3;
    public String str1;
    public String str2;
    public String str3;

    public prog() {
        initStorage();
    }

    public static void main(String[] args) {
        new prog().runProgram();
    }

    public int runProgram() {
        return runProgram(1, prog$Label.$EXIT$);
    }

    public int runProgram(int label) {
        return runProgram(label, label);
    }

    public int runProgram(int beginLabel, int endLabel) {
        int nextLabel = beginLabel;
        while(nextLabel != prog$Label.$EXIT$ && nextLabel <= endLabel) {
            switch(nextLabel) {
                case prog$Label.DISPLAY_VALUES:
                {
                    System.out.println(num1); // prog.cbl:20: DISPLAY
                    System.out.println(num2); // prog.cbl:21: DISPLAY
                    System.out.println(num3); // prog.cbl:22: DISPLAY
                    nextLabel = prog$Label.MOVE_VALUES;
                    break;
                }
                case prog$Label.MOVE_VALUES:
                {
                    num2 = num1; // prog.cbl:28 MOVE
                    System.out.println(num2); //prog.cbl:29: DISPLAY
                    num3 = num1; //prog.cbl:31: MOVE
                    System.out.println(num3); //prog.cbl:32: DISPLAY
                    str2 = str1.toString(); // prog.cbl:34: MOVE
                    System.out.println(str2); //prog.cbl:35: DISPLAY
                    str3 = str1.toString(); //prog.cbl:37: MOVE
                    System.out.println(str3); //prog.cbl:38: DISPLAY
                    num2 = Integer.parseInt(str1); //prog.cbl:40: MOVE
                    System.out.println(num2); //prog.cbl:41: DISPLAY
                    str2 = String.valueOf(num1); //prog.cbl:43: MOVE
                    System.out.println(str2); //prog.cbl:44: DISPLAY
                    nextLabel = prog$Label.IF_STATEMENTS;
                    break;
                }
                case prog$Label.IF_STATEMENTS:
                {
                    if (num1 == 12345) { // prog.cbl:47: IF
                        System.out.println("OK 001"); // prog.cbl:48: DISPLAY
                    }
                    if(str1.equals("hello")) { //prog.cbl:51: IF
                        System.out.println("OK 002"); // prog.cbl:52: DISPLAY
                    }
                    if(!str1.equals(String.valueOf(num1))) { //prog.cbl:55: IF
                        System.out.println("OK 003"); // prog.cbl:56: DISPLAY
                    }
                    nextLabel = prog$Label.LABEL_TEST_GOTO;
                    break;
                }
                case prog$Label.LABEL_TEST_GOTO:
                {
                    System.out.println("OK 004"); // prog.cbl:60: DISPLAY
                    nextLabel = prog$Label.LABEL_TEST_PERFORM; if(true) break; // prog.cbl:61: GO TO
                    nextLabel = prog$Label.LABEL_PERFORM_PROC;
                    break;
                }
                case prog$Label.LABEL_PERFORM_PROC:
                {
                    System.out.println("OK 007"); // prog.cbl:63: DISPLAY
                    nextLabel = prog$Label.LABEL_PERFORM_THRU_PROC1;
                    break;
                }
                case prog$Label.LABEL_PERFORM_THRU_PROC1:
                {
                    System.out.println("OK 005"); // prog.cbl:65: DISPLAY
                    nextLabel = prog$Label.LABEL_PERFORM_THRU_PROC2;
                    break;
                }
                case prog$Label.LABEL_PERFORM_THRU_PROC2:
                {
                    System.out.println("OK 006"); // prog.cbl:67: DISPLAY
                    nextLabel = prog$Label.LABEL_TEST_PERFORM;
                    break;
                }
                case prog$Label.LABEL_TEST_PERFORM:
                {
                    runProgram(prog$Label.LABEL_PERFORM_THRU_PROC1, prog$Label.LABEL_PERFORM_THRU_PROC2); // prog.cbl:69: PERFORM
                    runProgram(prog$Label.LABEL_PERFORM_PROC); // prog.cbl:71: PERFORM
                    System.out.println("OK 008"); // prog.cbl:72: DISPLAY
                    nextLabel = prog$Label.$EXIT$;
                    break;
                }
            }
        }
        return 0;
    }

    public prog initStorage() {
        num1 = 12345;
        num2 = 9876543;
        num3 = 456;
        str1 = "54321";
        str2 = "hello";
        str3 = "abc";
        return this;
    }

    public prog clearStorage() {
        num1 = 0;
        num2 = 0;
        num3 = 0;
        str1 = "";
        str2 = "";
        str3 = "";
        return this;
    }
}
