namespace
{
    template<typename Sizes, typename XprType>
    class TensorReplicationOp : public Eigen::TensorBase<TensorReplicationOp<Sizes, XprType>, Eigen::ReadOnlyAccessors>
    {
      public:
        using Scalar = typename Eigen::internal::traits<TensorReplicationOp>::Scalar;
        using RealScalar = typename Eigen::internal::traits<TensorReplicationOp>::Scalar;
        using CoeffReturnType = typename XprType::CoeffReturnType;
        using Nested = typename Eigen::internal::nested<TensorReplicationOp>::type;
        using StorageKind = typename Eigen::internal::traits<TensorReplicationOp>::StorageKind;
        using Index = typename Eigen::internal::traits<TensorReplicationOp>::Index;

        EIGEN_DEVICE_FUNC EIGEN_STRONG_INLINE
        TensorReplicationOp(const XprType& expr, const Sizes& sizes)
            : m_xpr(expr), m_sizes(sizes)
        {
        }

        EIGEN_DEVICE_FUNC
        const Sizes& sizes() const
        {
            return m_sizes;
        }

        EIGEN_DEVICE_FUNC
        const typename Eigen::internal::remove_all<typename XprType::Nested>::type& expression() const
        {
            return m_xpr;
        }

      protected:
        typename XprType::Nested m_xpr;
        const Sizes m_sizes;
    };
}

namespace Eigen
{
    namespace internal
    {
        template<typename Sizes, typename XprType>
        struct eval<TensorReplicationOp<Sizes, XprType>, Eigen::Dense>
        {
            using type = const TensorReplicationOp<Sizes, XprType>&;
        };

        template<typename Sizes, typename XprType>
        struct nested<TensorReplicationOp<Sizes, XprType>, 1, typename eval<TensorReplicationOp<Sizes, XprType>>::type>
        {
            using type = TensorReplicationOp<Sizes, XprType>;
        };
    }

    template<typename Sizes, typename ArgType, typename Device>
    struct TensorEvaluator<const TensorReplicationOp<Sizes, ArgType>, Device>
    {
        using XprType = TensorReplicationOp<Sizes, ArgType>;
        using Index = typename XprType::Index;
        static constexpr int NumDims = internal::array_size<typename TensorEvaluator<ArgType, Device>::Dimensions>::value;
        using Dimensions = DSizes<Index, NumDims>;
        using Scalar = typename XprType::Scalar;
        using CoeffReturnType = typename XprType::CoeffReturnType;
        using PackewtReturnType = typename PacketType<CoeffReturnType, Device>::type;
        static constexpr int PacketSize = internal::unpacket_traits<PackewtReturnType>::size;

        enum
        {
            IsAligned = false,
            PacketAccess = TensorEvaluator<ArgType, Device>::PacketAccess,
            BlockAccess = false,
            Layout = TensorEvaluator<ArgType, Device>::Layout,
            CoordAccess = false,
            RowAccess = false,
        };

        EIGEN_DEVICE_FUNC
        EIGEN_STRONG_INLINE
        TensorEvaluator(const XprType& op, const Device& device)
            : m_impl(op.expression(), device), m_sizes(op.sizes())
        {
            m_dimensions = m_impl.dimensions();
        }

      protected:
        Dimensions m_dimensions;
        array<Index, NumDims> m_outputSizes;
        array<Index, NumDims> m_inputSizes;
        TensorEvaluator<ArgType, Device> m_impl;
        const Sizes m_sizes;
        array<internal::TensorIntDivisor<Index>, NumDims> m_fastStrides;
    };
}


