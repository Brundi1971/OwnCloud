

#if !__AVR32_UC__ && !__AVR32_AP__
  #error Implementation of the AVR32 architecture not supported by the INTC driver.
#endif

#include <avr32/io.h>


  .section  .exception, "ax", @progbits
  .balign 0x200

  // Export symbol.
  .global _evba
  .type _evba, @function
_evba:

        .org  0x000
        // Unrecoverable Exception.
_handle_Unrecoverable_Exception:
        lda.w   pc, general_int_handler

        .org  0x004
        // TLB Multiple Hit: UNUSED IN AVR32UC.
_handle_TLB_Multiple_Hit:
        lda.w   pc, general_int_handler

        .org  0x008
        // Bus Error Data Fetch.
_handle_Bus_Error_Data_Fetch:
        lda.w   pc, general_int_handler

        .org  0x00C
         // Bus Error Instruction Fetch.
_handle_Bus_Error_Instruction_Fetch:
        lda.w   pc, Bus_Error_Instruction_Fetch_int_handler

        .org  0x010
        // NMI.
_handle_NMI:
        lda.w   pc, general_int_handler

        .org  0x014
        // Instruction Address.
_handle_Instruction_Address:
        lda.w   pc, general_int_handler

        .org  0x018
        // ITLB Protection.
_handle_ITLB_Protection:
        lda.w   pc, general_int_handler

        .org  0x01C
        // Breakpoint.
_handle_Breakpoint:
        lda.w   pc, general_int_handler

        .org  0x020
        // Illegal Opcode.
_handle_Illegal_Opcode:
        lda.w   pc, Illegal_Opcode_int_handler

        .org  0x024
        // Unimplemented Instruction.
_handle_Unimplemented_Instruction:
        lda.w   pc, general_int_handler

        .org  0x028
        // Privilege Violation.
_handle_Privilege_Violation:
        lda.w   pc, general_int_handler

        .org  0x02C
        // Floating-Point: UNUSED IN AVR32UC.
_handle_Floating_Point:
        rjmp $

        .org  0x030
        // Coprocessor Absent: UNUSED IN AVR32UC.
_handle_Coprocessor_Absent:
        lda.w   pc, general_int_handler

        .org  0x034
        // Data Address (Read).
_handle_Data_Address_Read:
        lda.w   pc, general_int_handler

        .org  0x038
        // Data Address (Write).
_handle_Data_Address_Write:
        lda.w   pc, general_int_handler

        .org  0x03C
        // DTLB Protection (Read).
_handle_DTLB_Protection_Read:
        lda.w   pc, general_int_handler

        .org  0x040
        // DTLB Protection (Write).
_handle_DTLB_Protection_Write:
        lda.w   pc, general_int_handler

        .org  0x044
        // DTLB Modified: UNUSED IN AVR32UC.
_handle_DTLB_Modified:
         lda.w   pc, general_int_handler

        .org  0x050
        // ITLB Miss: UNUSED IN AVR32UC.
_handle_ITLB_Miss:
        lda.w   pc, general_int_handler

        .org  0x060
        // DTLB Miss (Read): UNUSED IN AVR32UC.
_handle_DTLB_Miss_Read:
        lda.w   pc, general_int_handler

        .org  0x070
        // DTLB Miss (Write): UNUSED IN AVR32UC.
_handle_DTLB_Miss_Write:
        lda.w   pc, general_int_handler

        .org  0x100
        // Supervisor Call.
_handle_Supervisor_Call:
        lda.w   pc, general_int_handler


// Interrupt support.
// The interrupt controller must provide the offset address relative to EVBA.
// Important note:
//   All interrupts call a C function named _get_interrupt_handler.
//   This function will read group and interrupt line number to then return in
//   R12 a pointer to a user-provided interrupt handler.

  .balign 4

  .irp    priority, 0, 1, 2, 3
_int\priority:
#if __AVR32_UC__
  // R8-R12, LR, PC and SR are automatically pushed onto the system stack by the
  // CPU upon interrupt entry. No other register is saved by hardware.
#elif __AVR32_AP__
  // PC and SR are automatically saved in respectively RAR_INTx and RSR_INTx by
  // the CPU upon interrupt entry. No other register is saved by hardware.
  pushm   r8-r12, lr
#endif
  mov     r12, \priority  // Pass the int_lev parameter to the _get_interrupt_handler function.
  call    _get_interrupt_handler
  cp.w    r12, 0          // Get the pointer to the interrupt handler returned by the function.
#if __AVR32_UC__
  movne   pc, r12         // If this was not a spurious interrupt (R12 != NULL), jump to the handler.
#elif __AVR32_AP__
  breq    spint\priority  // If this was a spurious interrupt (R12 == NULL), branch.
  st.w    --sp, r12       // Push the pointer to the interrupt handler onto the system stack since no register may be altered.
  popm    r8-r12, lr, pc  // Restore registers and jump to the handler.
spint\priority:
  popm    r8-r12, lr
#endif
  rete                    // If this was a spurious interrupt (R12 == NULL), return from event handler.
  .endr


// Constant data area.

  .balign 4

  // Values to store in the interrupt priority registers for the various interrupt priority levels.
  // The interrupt priority registers contain the interrupt priority level and
  // the EVBA-relative interrupt vector offset.
  .global ipr_val
  .type ipr_val, @object
ipr_val:
  .word (AVR32_INTC_INT0 << AVR32_INTC_IPR_INTLEVEL_OFFSET) | (_int0 - _evba),\
        (AVR32_INTC_INT1 << AVR32_INTC_IPR_INTLEVEL_OFFSET) | (_int1 - _evba),\
        (AVR32_INTC_INT2 << AVR32_INTC_IPR_INTLEVEL_OFFSET) | (_int2 - _evba),\
        (AVR32_INTC_INT3 << AVR32_INTC_IPR_INTLEVEL_OFFSET) | (_int3 - _evba)


//! \endverbatim
//! @}
