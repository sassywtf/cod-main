#pragma once
#include <cstdint>

// Sometimes you need different sizes than your architecture
// For example 64 bit addresses on 32 bit programs
template< typename PtrType = uintptr_t >
struct address_base_t {
    // Inner pointer
    PtrType mPtr;

    // Creates a NULL address object
    address_base_t() : mPtr{} {};

    // Creates an address object with the given pointer
    // <param name="ptr">The address on which the object will be based on</param>
    address_base_t(PtrType ptr) : mPtr(ptr) {};

    // Creates an address object with the given pointer
    // <param name="ptr">The address on which the object will be based on</param>
    address_base_t(PtrType* ptr) : mPtr(PtrType(ptr)) {};

    // Creates an address object with the given pointer
    // <param name="ptr">The address on which the object will be based on</param>
    address_base_t(void* ptr) : mPtr(PtrType(ptr)) {};

    // Creates an address object with the given pointer
    // <param name="ptr">The address on which the object will be based on</param>
    address_base_t(const void* ptr) : mPtr(PtrType(ptr)) {};

    // Destroys the address object
    ~address_base_t() = default;

    // Whenever an address object is being passed into a function but it requires
    // a uintptr this function will be called
    inline operator PtrType() const {
        return mPtr;
    }

    // Whenever an address object is being passed into a function but it requires
    // a void* this function will be called
    inline operator void* () {
        return reinterpret_cast<void*>(mPtr);
    }

    // Returns the inner pointer
    // <returns>Inner pointer</returns>
    inline PtrType GetInner() const {
        return mPtr;
    }

    // Compares the inner pointer with the given address
    // <param name=in>Address that will be compared</param>
    // <returns>True if the addresses match</returns>
    template< typename T = address_base_t<PtrType> >
    inline bool Compare(T in) const {
        return mPtr == PtrType(in);
    }

    // Actions performed on self

    // Deref inner pointer
    // <param name="in">Times the pointer will be deref'd</param>
    // <returns>Current address object</returns>
    inline address_base_t<PtrType>& SelfGet(uint8_t in = 1) {
        mPtr = Get<PtrType>(in);
        return *this;
    }

    // Add offset to inner pointer
    // <param name="in">Offset that will be added</param>
    // <returns>Current address object</returns>
    inline address_base_t<PtrType>& SelfOffset(ptrdiff_t offset) {
        mPtr += offset;
        return *this;
    }

    // Follows a relative JMP instruction
    // <param name="offset">Offset at which the function address is</param>
    // <returns>Address object</returns>
    template< typename T = address_base_t<PtrType> >
    inline address_base_t<PtrType>& SelfJmp(ptrdiff_t offset = 0x1) {
        mPtr = Jmp(offset);
        return *this;
    }

    // Finds a specific opcode
    // <param name="opcode">Opcode that is being searched for</param>
    // <param name="offset">Offset that should be added to the resulting address</param>
    // <returns>Address object</returns>
    inline address_base_t<PtrType>& SelfFindOpcode(byte opcode, ptrdiff_t offset = 0x0) {
        mPtr = FindOpcode(opcode, offset);
        return *this;
    }

    // Finds a specific opcode sequence
    // <param name="opcodes">Opcodes to be searched</param>
    // <param name="offset">Offset that should be added to the resulting address</param>
    // <returns>Address object</returns>
    inline address_base_t<PtrType>& SelfFindOpcodeSeq(std::vector<byte> opcodes, ptrdiff_t offset = 0x0) {
        mPtr = FindOpcodeSeq(opcodes, offset);
        return *this;
    }

    // Set inner pointer to given value
    // <param name="in">Offset that will be added</param>
    // <returns>Current address object</returns>
    template< typename T = address_base_t<PtrType> >
    inline address_base_t<PtrType>& Set(T in) {
        mPtr = PtrType(in);
        return *this;
    }

    // Const actions

    // Returns a casted version of the inner pointer
    // <returns>Current address object</returns>
    template< typename T = PtrType >
    inline T Cast() {
        return T(mPtr);
    }

    // Deref inner pointer
    // <param name="in">Times the pointer will be deref'd</param>
    // <returns>Current address object</returns>
    template< typename T = address_base_t<PtrType> >
    inline T Get(uint8_t in = 1) {
        if (!mPtr)
            return T{};

        PtrType dummy = mPtr;

        while (in--)
            /// Check if pointer is still valid
            if (dummy)
                dummy = *reinterpret_cast<PtrType*>(dummy);

        return T(dummy);
    }

    // Add offset to inner pointer
    // <param name="in">Offset that will be added</param>
    // <returns>Address object</returns>
    template< typename T = address_base_t<PtrType> >
    inline T Offset(ptrdiff_t offset) {
        return T(mPtr + offset);
    }

    // Follows a relative JMP instruction
    // <param name="offset">Offset at which the function address is</param>
    // <returns>Address object</returns>
    template< typename T = address_base_t<PtrType> >
    inline T Jmp(ptrdiff_t offset = 0x1) {
        if (!mPtr)
            return T{};

        PtrType base = mPtr + offset;

        auto displacement = *reinterpret_cast<int32_t*>(base);

        base += sizeof(uint32_t);
        base += displacement;

        return T(base);
    }

    // Finds a specific opcode
    // <param name="opcode">Opcode to be searched</param>
    // <param name="offset">Offset that should be added to the resulting address</param>
    // <returns>Address object</returns>
    template< typename T = address_base_t<PtrType> >
    inline T FindOpcode(byte opcode, ptrdiff_t offset = 0x0) {
        if (!mPtr)
            return T{};

        auto base = mPtr;

        auto opcodeAtAddress = byte();

        while (opcodeAtAddress = *reinterpret_cast<byte*>(base)) {
            if (opcode == opcodeAtAddress)
                break;

            base += 1;
        }

        base += offset;

        return T(base);
    }

    // Finds a specific opcode sequence
    // <param name="opcode">Opcodes to be searched</param>
    // <param name="offset">Offset that should be added to the resulting address</param>
    // <returns>Address object</returns>
    template< typename T = address_base_t<PtrType> >
    inline T FindOpcodeSeq(std::vector<byte> opcodes, ptrdiff_t offset = 0x0) {
        if (!mPtr)
            return T{};

        auto base = mPtr;

        auto opcodeAtAddress = byte();

        while (opcodeAtAddress = *reinterpret_cast<byte*>(base)) {
            if (opcodes.at(0) == opcodeAtAddress) {
                for (auto i = 0u; i < opcodes.size(); i++)
                    if (opcodes.at(i) != *reinterpret_cast<byte*>(base + i))
                        goto CONT;

                break;
            }
        CONT:
            base += 1;
        }

        base += offset;

        return T(base);
    }
};

// Adjusted size to architecture
using address_t = address_base_t<uintptr_t>;

// 32 bit
using address_32_t = address_base_t<uint32_t>;

// 64 bit
using address_64_t = address_base_t<uint64_t>;
