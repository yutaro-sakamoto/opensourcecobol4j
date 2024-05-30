public class prog {

    private int num1;
    private int num2;
    private int num3;
    private String str1;
    private String str2;
    private String str3;

    private int returnCode;


    // label向けの定数
    enum CobolLabel {
        $DEFAULT$,
        DISPLAY_VALUES,
        MOVE_VALUES,
        IF_STATEMENTS,
        LABEL_TEST_GOTO,
        LABEL_PERFORM_PROC,
        LABEL_PERFORM_THRU_PROC1,
        LABEL_PERFORM_THRU_PROC2,
        LABEL_TEST_PERFORM,
        EXIT,
    }

    public prog() {
        initStorage();
    }

    public static void main(String[] args) {
        new prog().runProgram();
    }

    public int runProgram() {
        returnCode = 0;
        return runProgram(CobolLabel.DISPLAY_VALUES, CobolLabel.EXIT);
    }

    private int runProgram(CobolLabel label) {
        return runProgram(label, label);
    }

    private int runProgram(CobolLabel beginLabel, CobolLabel endLabel) {
        CobolLabel nextLabel = beginLabel;
        CobolLabel prevLabel = CobolLabel.$DEFAULT$;
        while(nextLabel != CobolLabel.EXIT && prevLabel != endLabel) {
            switch(nextLabel) {
                case DISPLAY_VALUES:
                {
                    System.out.println(num1); // prog.cbl:20: DISPLAY
                    System.out.println(num2); // prog.cbl:21: DISPLAY
                    System.out.println(num3); // prog.cbl:22: DISPLAY
                    prevLabel = nextLabel; nextLabel = CobolLabel.MOVE_VALUES;
                    break;
                }
                case MOVE_VALUES:
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
                    prevLabel = nextLabel; nextLabel = CobolLabel.IF_STATEMENTS;
                    break;
                }
                case IF_STATEMENTS:
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
                    prevLabel = nextLabel; nextLabel = CobolLabel.LABEL_TEST_GOTO;
                    break;
                }
                case LABEL_TEST_GOTO:
                {
                    System.out.println("OK 004"); // prog.cbl:60: DISPLAY
                    nextLabel = CobolLabel.LABEL_TEST_PERFORM; if(true) break; // prog.cbl:61: GO TO
                    prevLabel = nextLabel; nextLabel = CobolLabel.LABEL_PERFORM_PROC;
                    break;
                }
                case LABEL_PERFORM_PROC:
                {
                    System.out.println("OK 007"); // prog.cbl:63: DISPLAY
                    prevLabel = nextLabel; nextLabel = CobolLabel.LABEL_PERFORM_THRU_PROC1;
                    break;
                }
                case LABEL_PERFORM_THRU_PROC1:
                {
                    System.out.println("OK 005"); // prog.cbl:65: DISPLAY
                    prevLabel = nextLabel; nextLabel = CobolLabel.LABEL_PERFORM_THRU_PROC2;
                    break;
                }
                case LABEL_PERFORM_THRU_PROC2:
                {
                    System.out.println("OK 006"); // prog.cbl:67: DISPLAY
                    prevLabel = nextLabel; nextLabel = CobolLabel.LABEL_TEST_PERFORM;
                    break;
                }
                case LABEL_TEST_PERFORM:
                {
                    runProgram(CobolLabel.LABEL_PERFORM_THRU_PROC1, CobolLabel.LABEL_PERFORM_THRU_PROC2); // prog.cbl:69: PERFORM
                    runProgram(CobolLabel.LABEL_PERFORM_PROC); // prog.cbl:71: PERFORM
                    System.out.println("OK 008"); // prog.cbl:72: DISPLAY
                    prevLabel = nextLabel; nextLabel = CobolLabel.EXIT;
                    break;
                }
            }
        }
        return returnCode;
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
