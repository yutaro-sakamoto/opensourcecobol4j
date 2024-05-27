public class prog {

    private prog$Storage storage;

    public prog() {
        this.storage = new prog$Storage();
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
                    System.out.println(storage.num1); // prog.cbl:20: DISPLAY
                    System.out.println(storage.num2); // prog.cbl:21: DISPLAY
                    System.out.println(storage.num3); // prog.cbl:22: DISPLAY
                    nextLabel = prog$Label.MOVE_VALUES;
                    break;
                }
                case prog$Label.MOVE_VALUES:
                {
                    storage.num2 = storage.num1; // prog.cbl:28 MOVE
                    System.out.println(storage.num2); //prog.cbl:29: DISPLAY
                    storage.num3 = storage.num1; //prog.cbl:31: MOVE
                    System.out.println(storage.num3); //prog.cbl:32: DISPLAY
                    storage.str2 = storage.str1.toString(); // prog.cbl:34: MOVE
                    System.out.println(storage.str2); //prog.cbl:35: DISPLAY
                    storage.str3 = storage.str1.toString(); //prog.cbl:37: MOVE
                    System.out.println(storage.str3); //prog.cbl:38: DISPLAY
                    nextLabel = prog$Label.IF_STATEMENTS;
                    break;
                }
                case prog$Label.IF_STATEMENTS:
                {
                    if (storage.num1 == 12345) { // prog.cbl:41: IF
                        System.out.println("OK 001"); // prog.cbl:42: DISPLAY
                    }
                    if(storage.str1.equals("hello")) { //prog.cbl:45: IF
                        System.out.println("OK 002"); // prog.cbl:46: DISPLAY
                    }
                    if(!storage.str1.equals(String.valueOf(storage.num1))) { //prog.cbl:49: IF
                        System.out.println("OK 003"); // prog.cbl:50: DISPLAY
                    }
                    nextLabel = prog$Label.LABEL_TEST_GOTO;
                    break;
                }
                case prog$Label.LABEL_TEST_GOTO:
                {
                    System.out.println("OK 004"); // prog.cbl:54: DISPLAY
                    nextLabel = prog$Label.LABEL_TEST_PERFORM; if(true) break; // prog.cbl:55: GO TO
                    nextLabel = prog$Label.LABEL_PERFORM_PROC;
                    break;
                }
                case prog$Label.LABEL_PERFORM_PROC:
                {
                    System.out.println("OK 007"); // prog.cbl:57: DISPLAY
                    nextLabel = prog$Label.LABEL_PERFORM_THRU_PROC1;
                    break;
                }
                case prog$Label.LABEL_PERFORM_THRU_PROC1:
                {
                    System.out.println("OK 005"); // prog.cbl:59: DISPLAY
                    nextLabel = prog$Label.LABEL_PERFORM_THRU_PROC2;
                    break;
                }
                case prog$Label.LABEL_PERFORM_THRU_PROC2:
                {
                    System.out.println("OK 006"); // prog.cbl:61: DISPLAY
                    nextLabel = prog$Label.LABEL_TEST_PERFORM;
                    break;
                }
                case prog$Label.LABEL_TEST_PERFORM:
                {
                    runProgram(prog$Label.LABEL_PERFORM_THRU_PROC1, prog$Label.LABEL_PERFORM_THRU_PROC2); // prog.cbl:63: PERFORM
                    runProgram(prog$Label.LABEL_PERFORM_PROC); // prog.cbl:65: PERFORM
                    System.out.println("OK 008"); // prog.cbl:66j: DISPLAY
                    nextLabel = prog$Label.$EXIT$;
                    break;
                }
            }
        }
        return 0;
    }
}
