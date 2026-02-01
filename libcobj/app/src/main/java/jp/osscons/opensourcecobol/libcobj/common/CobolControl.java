/*
 * Copyright (C) 2021-2022 TOKYO SYSTEM HOUSE Co., Ltd.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public License
 * as published by the Free Software Foundation; either version 3.0,
 * or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; see the file COPYING.LIB.  If
 * not, write to the Free Software Foundation, 51 Franklin Street, Fifth Floor
 * Boston, MA 02110-1301 USA
 */
package jp.osscons.opensourcecobol.libcobj.common;

import java.util.Optional;
import jp.osscons.opensourcecobol.libcobj.exceptions.CobolRuntimeException;
import jp.osscons.opensourcecobol.libcobj.exceptions.CobolStopRunException;

/**
 * Control flow class for COBOL program execution.
 *
 * @deprecated This class is no longer used in switch-based code generation.
 *     Kept for backward compatibility with older generated code.
 */
@Deprecated
public abstract class CobolControl {
    /** Label type enumeration */
    public enum LabelType {
        /** Regular label/paragraph */
        label,
        /** Section label */
        section,
    }

    /**
     * Execute the label's code.
     *
     * @return The next label to execute, or empty if done
     * @throws CobolRuntimeException On runtime error
     * @throws CobolStopRunException On STOP RUN
     * @deprecated No longer used in switch-based execution
     */
    @Deprecated
    public abstract Optional<CobolControl> run()
            throws CobolRuntimeException, CobolStopRunException;

    /** Control ID */
    public int contId = -1;

    /** Label type */
    public LabelType type = LabelType.label;

    /** Default constructor */
    public CobolControl() {
        this.contId = -1;
        this.type = LabelType.label;
    }

    /**
     * Constructor with control ID and type.
     *
     * @param contId Control ID
     * @param type Label type
     */
    public CobolControl(int contId, LabelType type) {
        this.contId = contId;
        this.type = type;
    }

    /**
     * Returns a pure (no-op) control.
     *
     * @return A control that returns empty
     * @deprecated No longer used in switch-based execution
     */
    @Deprecated
    public static CobolControl pure() {
        return new CobolControl() {
            @Override
            public Optional<CobolControl> run()
                    throws CobolRuntimeException, CobolStopRunException {
                return Optional.empty();
            }
        };
    }

    /**
     * Creates a GO TO control.
     *
     * @param cont Target control
     * @return A control that jumps to target
     * @deprecated No longer used in switch-based execution
     */
    @Deprecated
    public static CobolControl goTo(CobolControl cont) {
        return new CobolControl() {
            @Override
            public Optional<CobolControl> run()
                    throws CobolRuntimeException, CobolStopRunException {
                return cont.run();
            }
        };
    }

    /**
     * Creates a PERFORM THRU control.
     *
     * @param contList Array of controls
     * @param begin Start label index
     * @param end End label index
     * @return A control that performs the range
     * @deprecated No longer used in switch-based execution
     */
    @Deprecated
    public static CobolControl performThrough(CobolControl[] contList, int begin, int end) {
        return new CobolControl() {
            @Override
            public Optional<CobolControl> run()
                    throws CobolRuntimeException, CobolStopRunException {
                Optional<CobolControl> nextCont = Optional.of(contList[begin]);
                LabelType endType = contList[end].type;
                int executedProgramId;
                do {
                    CobolControl cont = nextCont.get();
                    executedProgramId = cont.contId;
                    nextCont = cont.run();
                } while (nextCont.isPresent() && executedProgramId != end);

                if (endType == LabelType.section) {
                    while (nextCont.isPresent() && nextCont.get().type == LabelType.label) {
                        CobolControl cont = nextCont.get();
                        nextCont = cont.run();
                    }
                }
                return Optional.of(CobolControl.pure());
            }
        };
    }

    /**
     * Creates a PERFORM control.
     *
     * @param contList Array of controls
     * @param labelId Label index
     * @return A control that performs the label
     * @deprecated No longer used in switch-based execution
     */
    @Deprecated
    public static CobolControl perform(CobolControl[] contList, int labelId) {
        return CobolControl.performThrough(contList, labelId, labelId);
    }
}
