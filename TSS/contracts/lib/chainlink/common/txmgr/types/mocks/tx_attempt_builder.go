// Code generated by mockery v2.53.0. DO NOT EDIT.

package mocks

import (
	context "context"

	chains "github.com/smartcontractkit/chainlink-framework/chains"

	fees "github.com/smartcontractkit/chainlink-framework/chains/fees"

	logger "github.com/smartcontractkit/chainlink-common/pkg/logger"

	mock "github.com/stretchr/testify/mock"

	types "github.com/smartcontractkit/chainlink-framework/chains/txmgr/types"
)

// TxAttemptBuilder is an autogenerated mock type for the TxAttemptBuilder type
type TxAttemptBuilder[CID chains.ID, HEAD chains.Head[BHASH], ADDR chains.Hashable, THASH chains.Hashable, BHASH chains.Hashable, SEQ chains.Sequence, FEE fees.Fee] struct {
	mock.Mock
}

type TxAttemptBuilder_Expecter[CID chains.ID, HEAD chains.Head[BHASH], ADDR chains.Hashable, THASH chains.Hashable, BHASH chains.Hashable, SEQ chains.Sequence, FEE fees.Fee] struct {
	mock *mock.Mock
}

func (_m *TxAttemptBuilder[CID, HEAD, ADDR, THASH, BHASH, SEQ, FEE]) EXPECT() *TxAttemptBuilder_Expecter[CID, HEAD, ADDR, THASH, BHASH, SEQ, FEE] {
	return &TxAttemptBuilder_Expecter[CID, HEAD, ADDR, THASH, BHASH, SEQ, FEE]{mock: &_m.Mock}
}

// Close provides a mock function with no fields
func (_m *TxAttemptBuilder[CID, HEAD, ADDR, THASH, BHASH, SEQ, FEE]) Close() error {
	ret := _m.Called()

	if len(ret) == 0 {
		panic("no return value specified for Close")
	}

	var r0 error
	if rf, ok := ret.Get(0).(func() error); ok {
		r0 = rf()
	} else {
		r0 = ret.Error(0)
	}

	return r0
}

// TxAttemptBuilder_Close_Call is a *mock.Call that shadows Run/Return methods with type explicit version for method 'Close'
type TxAttemptBuilder_Close_Call[CID chains.ID, HEAD chains.Head[BHASH], ADDR chains.Hashable, THASH chains.Hashable, BHASH chains.Hashable, SEQ chains.Sequence, FEE fees.Fee] struct {
	*mock.Call
}

// Close is a helper method to define mock.On call
func (_e *TxAttemptBuilder_Expecter[CID, HEAD, ADDR, THASH, BHASH, SEQ, FEE]) Close() *TxAttemptBuilder_Close_Call[CID, HEAD, ADDR, THASH, BHASH, SEQ, FEE] {
	return &TxAttemptBuilder_Close_Call[CID, HEAD, ADDR, THASH, BHASH, SEQ, FEE]{Call: _e.mock.On("Close")}
}

func (_c *TxAttemptBuilder_Close_Call[CID, HEAD, ADDR, THASH, BHASH, SEQ, FEE]) Run(run func()) *TxAttemptBuilder_Close_Call[CID, HEAD, ADDR, THASH, BHASH, SEQ, FEE] {
	_c.Call.Run(func(args mock.Arguments) {
		run()
	})
	return _c
}

func (_c *TxAttemptBuilder_Close_Call[CID, HEAD, ADDR, THASH, BHASH, SEQ, FEE]) Return(_a0 error) *TxAttemptBuilder_Close_Call[CID, HEAD, ADDR, THASH, BHASH, SEQ, FEE] {
	_c.Call.Return(_a0)
	return _c
}

func (_c *TxAttemptBuilder_Close_Call[CID, HEAD, ADDR, THASH, BHASH, SEQ, FEE]) RunAndReturn(run func() error) *TxAttemptBuilder_Close_Call[CID, HEAD, ADDR, THASH, BHASH, SEQ, FEE] {
	_c.Call.Return(run)
	return _c
}

// HealthReport provides a mock function with no fields
func (_m *TxAttemptBuilder[CID, HEAD, ADDR, THASH, BHASH, SEQ, FEE]) HealthReport() map[string]error {
	ret := _m.Called()

	if len(ret) == 0 {
		panic("no return value specified for HealthReport")
	}

	var r0 map[string]error
	if rf, ok := ret.Get(0).(func() map[string]error); ok {
		r0 = rf()
	} else {
		if ret.Get(0) != nil {
			r0 = ret.Get(0).(map[string]error)
		}
	}

	return r0
}

// TxAttemptBuilder_HealthReport_Call is a *mock.Call that shadows Run/Return methods with type explicit version for method 'HealthReport'
type TxAttemptBuilder_HealthReport_Call[CID chains.ID, HEAD chains.Head[BHASH], ADDR chains.Hashable, THASH chains.Hashable, BHASH chains.Hashable, SEQ chains.Sequence, FEE fees.Fee] struct {
	*mock.Call
}

// HealthReport is a helper method to define mock.On call
func (_e *TxAttemptBuilder_Expecter[CID, HEAD, ADDR, THASH, BHASH, SEQ, FEE]) HealthReport() *TxAttemptBuilder_HealthReport_Call[CID, HEAD, ADDR, THASH, BHASH, SEQ, FEE] {
	return &TxAttemptBuilder_HealthReport_Call[CID, HEAD, ADDR, THASH, BHASH, SEQ, FEE]{Call: _e.mock.On("HealthReport")}
}

func (_c *TxAttemptBuilder_HealthReport_Call[CID, HEAD, ADDR, THASH, BHASH, SEQ, FEE]) Run(run func()) *TxAttemptBuilder_HealthReport_Call[CID, HEAD, ADDR, THASH, BHASH, SEQ, FEE] {
	_c.Call.Run(func(args mock.Arguments) {
		run()
	})
	return _c
}

func (_c *TxAttemptBuilder_HealthReport_Call[CID, HEAD, ADDR, THASH, BHASH, SEQ, FEE]) Return(_a0 map[string]error) *TxAttemptBuilder_HealthReport_Call[CID, HEAD, ADDR, THASH, BHASH, SEQ, FEE] {
	_c.Call.Return(_a0)
	return _c
}

func (_c *TxAttemptBuilder_HealthReport_Call[CID, HEAD, ADDR, THASH, BHASH, SEQ, FEE]) RunAndReturn(run func() map[string]error) *TxAttemptBuilder_HealthReport_Call[CID, HEAD, ADDR, THASH, BHASH, SEQ, FEE] {
	_c.Call.Return(run)
	return _c
}

// Name provides a mock function with no fields
func (_m *TxAttemptBuilder[CID, HEAD, ADDR, THASH, BHASH, SEQ, FEE]) Name() string {
	ret := _m.Called()

	if len(ret) == 0 {
		panic("no return value specified for Name")
	}

	var r0 string
	if rf, ok := ret.Get(0).(func() string); ok {
		r0 = rf()
	} else {
		r0 = ret.Get(0).(string)
	}

	return r0
}

// TxAttemptBuilder_Name_Call is a *mock.Call that shadows Run/Return methods with type explicit version for method 'Name'
type TxAttemptBuilder_Name_Call[CID chains.ID, HEAD chains.Head[BHASH], ADDR chains.Hashable, THASH chains.Hashable, BHASH chains.Hashable, SEQ chains.Sequence, FEE fees.Fee] struct {
	*mock.Call
}

// Name is a helper method to define mock.On call
func (_e *TxAttemptBuilder_Expecter[CID, HEAD, ADDR, THASH, BHASH, SEQ, FEE]) Name() *TxAttemptBuilder_Name_Call[CID, HEAD, ADDR, THASH, BHASH, SEQ, FEE] {
	return &TxAttemptBuilder_Name_Call[CID, HEAD, ADDR, THASH, BHASH, SEQ, FEE]{Call: _e.mock.On("Name")}
}

func (_c *TxAttemptBuilder_Name_Call[CID, HEAD, ADDR, THASH, BHASH, SEQ, FEE]) Run(run func()) *TxAttemptBuilder_Name_Call[CID, HEAD, ADDR, THASH, BHASH, SEQ, FEE] {
	_c.Call.Run(func(args mock.Arguments) {
		run()
	})
	return _c
}

func (_c *TxAttemptBuilder_Name_Call[CID, HEAD, ADDR, THASH, BHASH, SEQ, FEE]) Return(_a0 string) *TxAttemptBuilder_Name_Call[CID, HEAD, ADDR, THASH, BHASH, SEQ, FEE] {
	_c.Call.Return(_a0)
	return _c
}

func (_c *TxAttemptBuilder_Name_Call[CID, HEAD, ADDR, THASH, BHASH, SEQ, FEE]) RunAndReturn(run func() string) *TxAttemptBuilder_Name_Call[CID, HEAD, ADDR, THASH, BHASH, SEQ, FEE] {
	_c.Call.Return(run)
	return _c
}

// NewBumpTxAttempt provides a mock function with given fields: ctx, tx, previousAttempt, priorAttempts, lggr
func (_m *TxAttemptBuilder[CID, HEAD, ADDR, THASH, BHASH, SEQ, FEE]) NewBumpTxAttempt(ctx context.Context, tx types.Tx[CID, ADDR, THASH, BHASH, SEQ, FEE], previousAttempt types.TxAttempt[CID, ADDR, THASH, BHASH, SEQ, FEE], priorAttempts []types.TxAttempt[CID, ADDR, THASH, BHASH, SEQ, FEE], lggr logger.Logger) (types.TxAttempt[CID, ADDR, THASH, BHASH, SEQ, FEE], FEE, uint64, bool, error) {
	ret := _m.Called(ctx, tx, previousAttempt, priorAttempts, lggr)

	if len(ret) == 0 {
		panic("no return value specified for NewBumpTxAttempt")
	}

	var r0 types.TxAttempt[CID, ADDR, THASH, BHASH, SEQ, FEE]
	var r1 FEE
	var r2 uint64
	var r3 bool
	var r4 error
	if rf, ok := ret.Get(0).(func(context.Context, types.Tx[CID, ADDR, THASH, BHASH, SEQ, FEE], types.TxAttempt[CID, ADDR, THASH, BHASH, SEQ, FEE], []types.TxAttempt[CID, ADDR, THASH, BHASH, SEQ, FEE], logger.Logger) (types.TxAttempt[CID, ADDR, THASH, BHASH, SEQ, FEE], FEE, uint64, bool, error)); ok {
		return rf(ctx, tx, previousAttempt, priorAttempts, lggr)
	}
	if rf, ok := ret.Get(0).(func(context.Context, types.Tx[CID, ADDR, THASH, BHASH, SEQ, FEE], types.TxAttempt[CID, ADDR, THASH, BHASH, SEQ, FEE], []types.TxAttempt[CID, ADDR, THASH, BHASH, SEQ, FEE], logger.Logger) types.TxAttempt[CID, ADDR, THASH, BHASH, SEQ, FEE]); ok {
		r0 = rf(ctx, tx, previousAttempt, priorAttempts, lggr)
	} else {
		r0 = ret.Get(0).(types.TxAttempt[CID, ADDR, THASH, BHASH, SEQ, FEE])
	}

	if rf, ok := ret.Get(1).(func(context.Context, types.Tx[CID, ADDR, THASH, BHASH, SEQ, FEE], types.TxAttempt[CID, ADDR, THASH, BHASH, SEQ, FEE], []types.TxAttempt[CID, ADDR, THASH, BHASH, SEQ, FEE], logger.Logger) FEE); ok {
		r1 = rf(ctx, tx, previousAttempt, priorAttempts, lggr)
	} else {
		if ret.Get(1) != nil {
			r1 = ret.Get(1).(FEE)
		}
	}

	if rf, ok := ret.Get(2).(func(context.Context, types.Tx[CID, ADDR, THASH, BHASH, SEQ, FEE], types.TxAttempt[CID, ADDR, THASH, BHASH, SEQ, FEE], []types.TxAttempt[CID, ADDR, THASH, BHASH, SEQ, FEE], logger.Logger) uint64); ok {
		r2 = rf(ctx, tx, previousAttempt, priorAttempts, lggr)
	} else {
		r2 = ret.Get(2).(uint64)
	}

	if rf, ok := ret.Get(3).(func(context.Context, types.Tx[CID, ADDR, THASH, BHASH, SEQ, FEE], types.TxAttempt[CID, ADDR, THASH, BHASH, SEQ, FEE], []types.TxAttempt[CID, ADDR, THASH, BHASH, SEQ, FEE], logger.Logger) bool); ok {
		r3 = rf(ctx, tx, previousAttempt, priorAttempts, lggr)
	} else {
		r3 = ret.Get(3).(bool)
	}

	if rf, ok := ret.Get(4).(func(context.Context, types.Tx[CID, ADDR, THASH, BHASH, SEQ, FEE], types.TxAttempt[CID, ADDR, THASH, BHASH, SEQ, FEE], []types.TxAttempt[CID, ADDR, THASH, BHASH, SEQ, FEE], logger.Logger) error); ok {
		r4 = rf(ctx, tx, previousAttempt, priorAttempts, lggr)
	} else {
		r4 = ret.Error(4)
	}

	return r0, r1, r2, r3, r4
}

// TxAttemptBuilder_NewBumpTxAttempt_Call is a *mock.Call that shadows Run/Return methods with type explicit version for method 'NewBumpTxAttempt'
type TxAttemptBuilder_NewBumpTxAttempt_Call[CID chains.ID, HEAD chains.Head[BHASH], ADDR chains.Hashable, THASH chains.Hashable, BHASH chains.Hashable, SEQ chains.Sequence, FEE fees.Fee] struct {
	*mock.Call
}

// NewBumpTxAttempt is a helper method to define mock.On call
//   - ctx context.Context
//   - tx types.Tx[CID,ADDR,THASH,BHASH,SEQ,FEE]
//   - previousAttempt types.TxAttempt[CID,ADDR,THASH,BHASH,SEQ,FEE]
//   - priorAttempts []types.TxAttempt[CID,ADDR,THASH,BHASH,SEQ,FEE]
//   - lggr logger.Logger
func (_e *TxAttemptBuilder_Expecter[CID, HEAD, ADDR, THASH, BHASH, SEQ, FEE]) NewBumpTxAttempt(ctx interface{}, tx interface{}, previousAttempt interface{}, priorAttempts interface{}, lggr interface{}) *TxAttemptBuilder_NewBumpTxAttempt_Call[CID, HEAD, ADDR, THASH, BHASH, SEQ, FEE] {
	return &TxAttemptBuilder_NewBumpTxAttempt_Call[CID, HEAD, ADDR, THASH, BHASH, SEQ, FEE]{Call: _e.mock.On("NewBumpTxAttempt", ctx, tx, previousAttempt, priorAttempts, lggr)}
}

func (_c *TxAttemptBuilder_NewBumpTxAttempt_Call[CID, HEAD, ADDR, THASH, BHASH, SEQ, FEE]) Run(run func(ctx context.Context, tx types.Tx[CID, ADDR, THASH, BHASH, SEQ, FEE], previousAttempt types.TxAttempt[CID, ADDR, THASH, BHASH, SEQ, FEE], priorAttempts []types.TxAttempt[CID, ADDR, THASH, BHASH, SEQ, FEE], lggr logger.Logger)) *TxAttemptBuilder_NewBumpTxAttempt_Call[CID, HEAD, ADDR, THASH, BHASH, SEQ, FEE] {
	_c.Call.Run(func(args mock.Arguments) {
		run(args[0].(context.Context), args[1].(types.Tx[CID, ADDR, THASH, BHASH, SEQ, FEE]), args[2].(types.TxAttempt[CID, ADDR, THASH, BHASH, SEQ, FEE]), args[3].([]types.TxAttempt[CID, ADDR, THASH, BHASH, SEQ, FEE]), args[4].(logger.Logger))
	})
	return _c
}

func (_c *TxAttemptBuilder_NewBumpTxAttempt_Call[CID, HEAD, ADDR, THASH, BHASH, SEQ, FEE]) Return(attempt types.TxAttempt[CID, ADDR, THASH, BHASH, SEQ, FEE], bumpedFee FEE, bumpedFeeLimit uint64, retryable bool, err error) *TxAttemptBuilder_NewBumpTxAttempt_Call[CID, HEAD, ADDR, THASH, BHASH, SEQ, FEE] {
	_c.Call.Return(attempt, bumpedFee, bumpedFeeLimit, retryable, err)
	return _c
}

func (_c *TxAttemptBuilder_NewBumpTxAttempt_Call[CID, HEAD, ADDR, THASH, BHASH, SEQ, FEE]) RunAndReturn(run func(context.Context, types.Tx[CID, ADDR, THASH, BHASH, SEQ, FEE], types.TxAttempt[CID, ADDR, THASH, BHASH, SEQ, FEE], []types.TxAttempt[CID, ADDR, THASH, BHASH, SEQ, FEE], logger.Logger) (types.TxAttempt[CID, ADDR, THASH, BHASH, SEQ, FEE], FEE, uint64, bool, error)) *TxAttemptBuilder_NewBumpTxAttempt_Call[CID, HEAD, ADDR, THASH, BHASH, SEQ, FEE] {
	_c.Call.Return(run)
	return _c
}

// NewCustomTxAttempt provides a mock function with given fields: ctx, tx, fee, gasLimit, txType, lggr
func (_m *TxAttemptBuilder[CID, HEAD, ADDR, THASH, BHASH, SEQ, FEE]) NewCustomTxAttempt(ctx context.Context, tx types.Tx[CID, ADDR, THASH, BHASH, SEQ, FEE], fee FEE, gasLimit uint64, txType int, lggr logger.Logger) (types.TxAttempt[CID, ADDR, THASH, BHASH, SEQ, FEE], bool, error) {
	ret := _m.Called(ctx, tx, fee, gasLimit, txType, lggr)

	if len(ret) == 0 {
		panic("no return value specified for NewCustomTxAttempt")
	}

	var r0 types.TxAttempt[CID, ADDR, THASH, BHASH, SEQ, FEE]
	var r1 bool
	var r2 error
	if rf, ok := ret.Get(0).(func(context.Context, types.Tx[CID, ADDR, THASH, BHASH, SEQ, FEE], FEE, uint64, int, logger.Logger) (types.TxAttempt[CID, ADDR, THASH, BHASH, SEQ, FEE], bool, error)); ok {
		return rf(ctx, tx, fee, gasLimit, txType, lggr)
	}
	if rf, ok := ret.Get(0).(func(context.Context, types.Tx[CID, ADDR, THASH, BHASH, SEQ, FEE], FEE, uint64, int, logger.Logger) types.TxAttempt[CID, ADDR, THASH, BHASH, SEQ, FEE]); ok {
		r0 = rf(ctx, tx, fee, gasLimit, txType, lggr)
	} else {
		r0 = ret.Get(0).(types.TxAttempt[CID, ADDR, THASH, BHASH, SEQ, FEE])
	}

	if rf, ok := ret.Get(1).(func(context.Context, types.Tx[CID, ADDR, THASH, BHASH, SEQ, FEE], FEE, uint64, int, logger.Logger) bool); ok {
		r1 = rf(ctx, tx, fee, gasLimit, txType, lggr)
	} else {
		r1 = ret.Get(1).(bool)
	}

	if rf, ok := ret.Get(2).(func(context.Context, types.Tx[CID, ADDR, THASH, BHASH, SEQ, FEE], FEE, uint64, int, logger.Logger) error); ok {
		r2 = rf(ctx, tx, fee, gasLimit, txType, lggr)
	} else {
		r2 = ret.Error(2)
	}

	return r0, r1, r2
}

// TxAttemptBuilder_NewCustomTxAttempt_Call is a *mock.Call that shadows Run/Return methods with type explicit version for method 'NewCustomTxAttempt'
type TxAttemptBuilder_NewCustomTxAttempt_Call[CID chains.ID, HEAD chains.Head[BHASH], ADDR chains.Hashable, THASH chains.Hashable, BHASH chains.Hashable, SEQ chains.Sequence, FEE fees.Fee] struct {
	*mock.Call
}

// NewCustomTxAttempt is a helper method to define mock.On call
//   - ctx context.Context
//   - tx types.Tx[CID,ADDR,THASH,BHASH,SEQ,FEE]
//   - fee FEE
//   - gasLimit uint64
//   - txType int
//   - lggr logger.Logger
func (_e *TxAttemptBuilder_Expecter[CID, HEAD, ADDR, THASH, BHASH, SEQ, FEE]) NewCustomTxAttempt(ctx interface{}, tx interface{}, fee interface{}, gasLimit interface{}, txType interface{}, lggr interface{}) *TxAttemptBuilder_NewCustomTxAttempt_Call[CID, HEAD, ADDR, THASH, BHASH, SEQ, FEE] {
	return &TxAttemptBuilder_NewCustomTxAttempt_Call[CID, HEAD, ADDR, THASH, BHASH, SEQ, FEE]{Call: _e.mock.On("NewCustomTxAttempt", ctx, tx, fee, gasLimit, txType, lggr)}
}

func (_c *TxAttemptBuilder_NewCustomTxAttempt_Call[CID, HEAD, ADDR, THASH, BHASH, SEQ, FEE]) Run(run func(ctx context.Context, tx types.Tx[CID, ADDR, THASH, BHASH, SEQ, FEE], fee FEE, gasLimit uint64, txType int, lggr logger.Logger)) *TxAttemptBuilder_NewCustomTxAttempt_Call[CID, HEAD, ADDR, THASH, BHASH, SEQ, FEE] {
	_c.Call.Run(func(args mock.Arguments) {
		run(args[0].(context.Context), args[1].(types.Tx[CID, ADDR, THASH, BHASH, SEQ, FEE]), args[2].(FEE), args[3].(uint64), args[4].(int), args[5].(logger.Logger))
	})
	return _c
}

func (_c *TxAttemptBuilder_NewCustomTxAttempt_Call[CID, HEAD, ADDR, THASH, BHASH, SEQ, FEE]) Return(attempt types.TxAttempt[CID, ADDR, THASH, BHASH, SEQ, FEE], retryable bool, err error) *TxAttemptBuilder_NewCustomTxAttempt_Call[CID, HEAD, ADDR, THASH, BHASH, SEQ, FEE] {
	_c.Call.Return(attempt, retryable, err)
	return _c
}

func (_c *TxAttemptBuilder_NewCustomTxAttempt_Call[CID, HEAD, ADDR, THASH, BHASH, SEQ, FEE]) RunAndReturn(run func(context.Context, types.Tx[CID, ADDR, THASH, BHASH, SEQ, FEE], FEE, uint64, int, logger.Logger) (types.TxAttempt[CID, ADDR, THASH, BHASH, SEQ, FEE], bool, error)) *TxAttemptBuilder_NewCustomTxAttempt_Call[CID, HEAD, ADDR, THASH, BHASH, SEQ, FEE] {
	_c.Call.Return(run)
	return _c
}

// NewEmptyTxAttempt provides a mock function with given fields: ctx, seq, feeLimit, fee, fromAddress
func (_m *TxAttemptBuilder[CID, HEAD, ADDR, THASH, BHASH, SEQ, FEE]) NewEmptyTxAttempt(ctx context.Context, seq SEQ, feeLimit uint64, fee FEE, fromAddress ADDR) (types.TxAttempt[CID, ADDR, THASH, BHASH, SEQ, FEE], error) {
	ret := _m.Called(ctx, seq, feeLimit, fee, fromAddress)

	if len(ret) == 0 {
		panic("no return value specified for NewEmptyTxAttempt")
	}

	var r0 types.TxAttempt[CID, ADDR, THASH, BHASH, SEQ, FEE]
	var r1 error
	if rf, ok := ret.Get(0).(func(context.Context, SEQ, uint64, FEE, ADDR) (types.TxAttempt[CID, ADDR, THASH, BHASH, SEQ, FEE], error)); ok {
		return rf(ctx, seq, feeLimit, fee, fromAddress)
	}
	if rf, ok := ret.Get(0).(func(context.Context, SEQ, uint64, FEE, ADDR) types.TxAttempt[CID, ADDR, THASH, BHASH, SEQ, FEE]); ok {
		r0 = rf(ctx, seq, feeLimit, fee, fromAddress)
	} else {
		r0 = ret.Get(0).(types.TxAttempt[CID, ADDR, THASH, BHASH, SEQ, FEE])
	}

	if rf, ok := ret.Get(1).(func(context.Context, SEQ, uint64, FEE, ADDR) error); ok {
		r1 = rf(ctx, seq, feeLimit, fee, fromAddress)
	} else {
		r1 = ret.Error(1)
	}

	return r0, r1
}

// TxAttemptBuilder_NewEmptyTxAttempt_Call is a *mock.Call that shadows Run/Return methods with type explicit version for method 'NewEmptyTxAttempt'
type TxAttemptBuilder_NewEmptyTxAttempt_Call[CID chains.ID, HEAD chains.Head[BHASH], ADDR chains.Hashable, THASH chains.Hashable, BHASH chains.Hashable, SEQ chains.Sequence, FEE fees.Fee] struct {
	*mock.Call
}

// NewEmptyTxAttempt is a helper method to define mock.On call
//   - ctx context.Context
//   - seq SEQ
//   - feeLimit uint64
//   - fee FEE
//   - fromAddress ADDR
func (_e *TxAttemptBuilder_Expecter[CID, HEAD, ADDR, THASH, BHASH, SEQ, FEE]) NewEmptyTxAttempt(ctx interface{}, seq interface{}, feeLimit interface{}, fee interface{}, fromAddress interface{}) *TxAttemptBuilder_NewEmptyTxAttempt_Call[CID, HEAD, ADDR, THASH, BHASH, SEQ, FEE] {
	return &TxAttemptBuilder_NewEmptyTxAttempt_Call[CID, HEAD, ADDR, THASH, BHASH, SEQ, FEE]{Call: _e.mock.On("NewEmptyTxAttempt", ctx, seq, feeLimit, fee, fromAddress)}
}

func (_c *TxAttemptBuilder_NewEmptyTxAttempt_Call[CID, HEAD, ADDR, THASH, BHASH, SEQ, FEE]) Run(run func(ctx context.Context, seq SEQ, feeLimit uint64, fee FEE, fromAddress ADDR)) *TxAttemptBuilder_NewEmptyTxAttempt_Call[CID, HEAD, ADDR, THASH, BHASH, SEQ, FEE] {
	_c.Call.Run(func(args mock.Arguments) {
		run(args[0].(context.Context), args[1].(SEQ), args[2].(uint64), args[3].(FEE), args[4].(ADDR))
	})
	return _c
}

func (_c *TxAttemptBuilder_NewEmptyTxAttempt_Call[CID, HEAD, ADDR, THASH, BHASH, SEQ, FEE]) Return(attempt types.TxAttempt[CID, ADDR, THASH, BHASH, SEQ, FEE], err error) *TxAttemptBuilder_NewEmptyTxAttempt_Call[CID, HEAD, ADDR, THASH, BHASH, SEQ, FEE] {
	_c.Call.Return(attempt, err)
	return _c
}

func (_c *TxAttemptBuilder_NewEmptyTxAttempt_Call[CID, HEAD, ADDR, THASH, BHASH, SEQ, FEE]) RunAndReturn(run func(context.Context, SEQ, uint64, FEE, ADDR) (types.TxAttempt[CID, ADDR, THASH, BHASH, SEQ, FEE], error)) *TxAttemptBuilder_NewEmptyTxAttempt_Call[CID, HEAD, ADDR, THASH, BHASH, SEQ, FEE] {
	_c.Call.Return(run)
	return _c
}

// NewPurgeTxAttempt provides a mock function with given fields: ctx, etx, lggr
func (_m *TxAttemptBuilder[CID, HEAD, ADDR, THASH, BHASH, SEQ, FEE]) NewPurgeTxAttempt(ctx context.Context, etx types.Tx[CID, ADDR, THASH, BHASH, SEQ, FEE], lggr logger.Logger) (types.TxAttempt[CID, ADDR, THASH, BHASH, SEQ, FEE], error) {
	ret := _m.Called(ctx, etx, lggr)

	if len(ret) == 0 {
		panic("no return value specified for NewPurgeTxAttempt")
	}

	var r0 types.TxAttempt[CID, ADDR, THASH, BHASH, SEQ, FEE]
	var r1 error
	if rf, ok := ret.Get(0).(func(context.Context, types.Tx[CID, ADDR, THASH, BHASH, SEQ, FEE], logger.Logger) (types.TxAttempt[CID, ADDR, THASH, BHASH, SEQ, FEE], error)); ok {
		return rf(ctx, etx, lggr)
	}
	if rf, ok := ret.Get(0).(func(context.Context, types.Tx[CID, ADDR, THASH, BHASH, SEQ, FEE], logger.Logger) types.TxAttempt[CID, ADDR, THASH, BHASH, SEQ, FEE]); ok {
		r0 = rf(ctx, etx, lggr)
	} else {
		r0 = ret.Get(0).(types.TxAttempt[CID, ADDR, THASH, BHASH, SEQ, FEE])
	}

	if rf, ok := ret.Get(1).(func(context.Context, types.Tx[CID, ADDR, THASH, BHASH, SEQ, FEE], logger.Logger) error); ok {
		r1 = rf(ctx, etx, lggr)
	} else {
		r1 = ret.Error(1)
	}

	return r0, r1
}

// TxAttemptBuilder_NewPurgeTxAttempt_Call is a *mock.Call that shadows Run/Return methods with type explicit version for method 'NewPurgeTxAttempt'
type TxAttemptBuilder_NewPurgeTxAttempt_Call[CID chains.ID, HEAD chains.Head[BHASH], ADDR chains.Hashable, THASH chains.Hashable, BHASH chains.Hashable, SEQ chains.Sequence, FEE fees.Fee] struct {
	*mock.Call
}

// NewPurgeTxAttempt is a helper method to define mock.On call
//   - ctx context.Context
//   - etx types.Tx[CID,ADDR,THASH,BHASH,SEQ,FEE]
//   - lggr logger.Logger
func (_e *TxAttemptBuilder_Expecter[CID, HEAD, ADDR, THASH, BHASH, SEQ, FEE]) NewPurgeTxAttempt(ctx interface{}, etx interface{}, lggr interface{}) *TxAttemptBuilder_NewPurgeTxAttempt_Call[CID, HEAD, ADDR, THASH, BHASH, SEQ, FEE] {
	return &TxAttemptBuilder_NewPurgeTxAttempt_Call[CID, HEAD, ADDR, THASH, BHASH, SEQ, FEE]{Call: _e.mock.On("NewPurgeTxAttempt", ctx, etx, lggr)}
}

func (_c *TxAttemptBuilder_NewPurgeTxAttempt_Call[CID, HEAD, ADDR, THASH, BHASH, SEQ, FEE]) Run(run func(ctx context.Context, etx types.Tx[CID, ADDR, THASH, BHASH, SEQ, FEE], lggr logger.Logger)) *TxAttemptBuilder_NewPurgeTxAttempt_Call[CID, HEAD, ADDR, THASH, BHASH, SEQ, FEE] {
	_c.Call.Run(func(args mock.Arguments) {
		run(args[0].(context.Context), args[1].(types.Tx[CID, ADDR, THASH, BHASH, SEQ, FEE]), args[2].(logger.Logger))
	})
	return _c
}

func (_c *TxAttemptBuilder_NewPurgeTxAttempt_Call[CID, HEAD, ADDR, THASH, BHASH, SEQ, FEE]) Return(attempt types.TxAttempt[CID, ADDR, THASH, BHASH, SEQ, FEE], err error) *TxAttemptBuilder_NewPurgeTxAttempt_Call[CID, HEAD, ADDR, THASH, BHASH, SEQ, FEE] {
	_c.Call.Return(attempt, err)
	return _c
}

func (_c *TxAttemptBuilder_NewPurgeTxAttempt_Call[CID, HEAD, ADDR, THASH, BHASH, SEQ, FEE]) RunAndReturn(run func(context.Context, types.Tx[CID, ADDR, THASH, BHASH, SEQ, FEE], logger.Logger) (types.TxAttempt[CID, ADDR, THASH, BHASH, SEQ, FEE], error)) *TxAttemptBuilder_NewPurgeTxAttempt_Call[CID, HEAD, ADDR, THASH, BHASH, SEQ, FEE] {
	_c.Call.Return(run)
	return _c
}

// NewTxAttempt provides a mock function with given fields: ctx, tx, lggr, opts
func (_m *TxAttemptBuilder[CID, HEAD, ADDR, THASH, BHASH, SEQ, FEE]) NewTxAttempt(ctx context.Context, tx types.Tx[CID, ADDR, THASH, BHASH, SEQ, FEE], lggr logger.Logger, opts ...fees.Opt) (types.TxAttempt[CID, ADDR, THASH, BHASH, SEQ, FEE], FEE, uint64, bool, error) {
	_va := make([]interface{}, len(opts))
	for _i := range opts {
		_va[_i] = opts[_i]
	}
	var _ca []interface{}
	_ca = append(_ca, ctx, tx, lggr)
	_ca = append(_ca, _va...)
	ret := _m.Called(_ca...)

	if len(ret) == 0 {
		panic("no return value specified for NewTxAttempt")
	}

	var r0 types.TxAttempt[CID, ADDR, THASH, BHASH, SEQ, FEE]
	var r1 FEE
	var r2 uint64
	var r3 bool
	var r4 error
	if rf, ok := ret.Get(0).(func(context.Context, types.Tx[CID, ADDR, THASH, BHASH, SEQ, FEE], logger.Logger, ...fees.Opt) (types.TxAttempt[CID, ADDR, THASH, BHASH, SEQ, FEE], FEE, uint64, bool, error)); ok {
		return rf(ctx, tx, lggr, opts...)
	}
	if rf, ok := ret.Get(0).(func(context.Context, types.Tx[CID, ADDR, THASH, BHASH, SEQ, FEE], logger.Logger, ...fees.Opt) types.TxAttempt[CID, ADDR, THASH, BHASH, SEQ, FEE]); ok {
		r0 = rf(ctx, tx, lggr, opts...)
	} else {
		r0 = ret.Get(0).(types.TxAttempt[CID, ADDR, THASH, BHASH, SEQ, FEE])
	}

	if rf, ok := ret.Get(1).(func(context.Context, types.Tx[CID, ADDR, THASH, BHASH, SEQ, FEE], logger.Logger, ...fees.Opt) FEE); ok {
		r1 = rf(ctx, tx, lggr, opts...)
	} else {
		if ret.Get(1) != nil {
			r1 = ret.Get(1).(FEE)
		}
	}

	if rf, ok := ret.Get(2).(func(context.Context, types.Tx[CID, ADDR, THASH, BHASH, SEQ, FEE], logger.Logger, ...fees.Opt) uint64); ok {
		r2 = rf(ctx, tx, lggr, opts...)
	} else {
		r2 = ret.Get(2).(uint64)
	}

	if rf, ok := ret.Get(3).(func(context.Context, types.Tx[CID, ADDR, THASH, BHASH, SEQ, FEE], logger.Logger, ...fees.Opt) bool); ok {
		r3 = rf(ctx, tx, lggr, opts...)
	} else {
		r3 = ret.Get(3).(bool)
	}

	if rf, ok := ret.Get(4).(func(context.Context, types.Tx[CID, ADDR, THASH, BHASH, SEQ, FEE], logger.Logger, ...fees.Opt) error); ok {
		r4 = rf(ctx, tx, lggr, opts...)
	} else {
		r4 = ret.Error(4)
	}

	return r0, r1, r2, r3, r4
}

// TxAttemptBuilder_NewTxAttempt_Call is a *mock.Call that shadows Run/Return methods with type explicit version for method 'NewTxAttempt'
type TxAttemptBuilder_NewTxAttempt_Call[CID chains.ID, HEAD chains.Head[BHASH], ADDR chains.Hashable, THASH chains.Hashable, BHASH chains.Hashable, SEQ chains.Sequence, FEE fees.Fee] struct {
	*mock.Call
}

// NewTxAttempt is a helper method to define mock.On call
//   - ctx context.Context
//   - tx types.Tx[CID,ADDR,THASH,BHASH,SEQ,FEE]
//   - lggr logger.Logger
//   - opts ...fees.Opt
func (_e *TxAttemptBuilder_Expecter[CID, HEAD, ADDR, THASH, BHASH, SEQ, FEE]) NewTxAttempt(ctx interface{}, tx interface{}, lggr interface{}, opts ...interface{}) *TxAttemptBuilder_NewTxAttempt_Call[CID, HEAD, ADDR, THASH, BHASH, SEQ, FEE] {
	return &TxAttemptBuilder_NewTxAttempt_Call[CID, HEAD, ADDR, THASH, BHASH, SEQ, FEE]{Call: _e.mock.On("NewTxAttempt",
		append([]interface{}{ctx, tx, lggr}, opts...)...)}
}

func (_c *TxAttemptBuilder_NewTxAttempt_Call[CID, HEAD, ADDR, THASH, BHASH, SEQ, FEE]) Run(run func(ctx context.Context, tx types.Tx[CID, ADDR, THASH, BHASH, SEQ, FEE], lggr logger.Logger, opts ...fees.Opt)) *TxAttemptBuilder_NewTxAttempt_Call[CID, HEAD, ADDR, THASH, BHASH, SEQ, FEE] {
	_c.Call.Run(func(args mock.Arguments) {
		variadicArgs := make([]fees.Opt, len(args)-3)
		for i, a := range args[3:] {
			if a != nil {
				variadicArgs[i] = a.(fees.Opt)
			}
		}
		run(args[0].(context.Context), args[1].(types.Tx[CID, ADDR, THASH, BHASH, SEQ, FEE]), args[2].(logger.Logger), variadicArgs...)
	})
	return _c
}

func (_c *TxAttemptBuilder_NewTxAttempt_Call[CID, HEAD, ADDR, THASH, BHASH, SEQ, FEE]) Return(attempt types.TxAttempt[CID, ADDR, THASH, BHASH, SEQ, FEE], fee FEE, feeLimit uint64, retryable bool, err error) *TxAttemptBuilder_NewTxAttempt_Call[CID, HEAD, ADDR, THASH, BHASH, SEQ, FEE] {
	_c.Call.Return(attempt, fee, feeLimit, retryable, err)
	return _c
}

func (_c *TxAttemptBuilder_NewTxAttempt_Call[CID, HEAD, ADDR, THASH, BHASH, SEQ, FEE]) RunAndReturn(run func(context.Context, types.Tx[CID, ADDR, THASH, BHASH, SEQ, FEE], logger.Logger, ...fees.Opt) (types.TxAttempt[CID, ADDR, THASH, BHASH, SEQ, FEE], FEE, uint64, bool, error)) *TxAttemptBuilder_NewTxAttempt_Call[CID, HEAD, ADDR, THASH, BHASH, SEQ, FEE] {
	_c.Call.Return(run)
	return _c
}

// NewTxAttemptWithType provides a mock function with given fields: ctx, tx, lggr, txType, opts
func (_m *TxAttemptBuilder[CID, HEAD, ADDR, THASH, BHASH, SEQ, FEE]) NewTxAttemptWithType(ctx context.Context, tx types.Tx[CID, ADDR, THASH, BHASH, SEQ, FEE], lggr logger.Logger, txType int, opts ...fees.Opt) (types.TxAttempt[CID, ADDR, THASH, BHASH, SEQ, FEE], FEE, uint64, bool, error) {
	_va := make([]interface{}, len(opts))
	for _i := range opts {
		_va[_i] = opts[_i]
	}
	var _ca []interface{}
	_ca = append(_ca, ctx, tx, lggr, txType)
	_ca = append(_ca, _va...)
	ret := _m.Called(_ca...)

	if len(ret) == 0 {
		panic("no return value specified for NewTxAttemptWithType")
	}

	var r0 types.TxAttempt[CID, ADDR, THASH, BHASH, SEQ, FEE]
	var r1 FEE
	var r2 uint64
	var r3 bool
	var r4 error
	if rf, ok := ret.Get(0).(func(context.Context, types.Tx[CID, ADDR, THASH, BHASH, SEQ, FEE], logger.Logger, int, ...fees.Opt) (types.TxAttempt[CID, ADDR, THASH, BHASH, SEQ, FEE], FEE, uint64, bool, error)); ok {
		return rf(ctx, tx, lggr, txType, opts...)
	}
	if rf, ok := ret.Get(0).(func(context.Context, types.Tx[CID, ADDR, THASH, BHASH, SEQ, FEE], logger.Logger, int, ...fees.Opt) types.TxAttempt[CID, ADDR, THASH, BHASH, SEQ, FEE]); ok {
		r0 = rf(ctx, tx, lggr, txType, opts...)
	} else {
		r0 = ret.Get(0).(types.TxAttempt[CID, ADDR, THASH, BHASH, SEQ, FEE])
	}

	if rf, ok := ret.Get(1).(func(context.Context, types.Tx[CID, ADDR, THASH, BHASH, SEQ, FEE], logger.Logger, int, ...fees.Opt) FEE); ok {
		r1 = rf(ctx, tx, lggr, txType, opts...)
	} else {
		if ret.Get(1) != nil {
			r1 = ret.Get(1).(FEE)
		}
	}

	if rf, ok := ret.Get(2).(func(context.Context, types.Tx[CID, ADDR, THASH, BHASH, SEQ, FEE], logger.Logger, int, ...fees.Opt) uint64); ok {
		r2 = rf(ctx, tx, lggr, txType, opts...)
	} else {
		r2 = ret.Get(2).(uint64)
	}

	if rf, ok := ret.Get(3).(func(context.Context, types.Tx[CID, ADDR, THASH, BHASH, SEQ, FEE], logger.Logger, int, ...fees.Opt) bool); ok {
		r3 = rf(ctx, tx, lggr, txType, opts...)
	} else {
		r3 = ret.Get(3).(bool)
	}

	if rf, ok := ret.Get(4).(func(context.Context, types.Tx[CID, ADDR, THASH, BHASH, SEQ, FEE], logger.Logger, int, ...fees.Opt) error); ok {
		r4 = rf(ctx, tx, lggr, txType, opts...)
	} else {
		r4 = ret.Error(4)
	}

	return r0, r1, r2, r3, r4
}

// TxAttemptBuilder_NewTxAttemptWithType_Call is a *mock.Call that shadows Run/Return methods with type explicit version for method 'NewTxAttemptWithType'
type TxAttemptBuilder_NewTxAttemptWithType_Call[CID chains.ID, HEAD chains.Head[BHASH], ADDR chains.Hashable, THASH chains.Hashable, BHASH chains.Hashable, SEQ chains.Sequence, FEE fees.Fee] struct {
	*mock.Call
}

// NewTxAttemptWithType is a helper method to define mock.On call
//   - ctx context.Context
//   - tx types.Tx[CID,ADDR,THASH,BHASH,SEQ,FEE]
//   - lggr logger.Logger
//   - txType int
//   - opts ...fees.Opt
func (_e *TxAttemptBuilder_Expecter[CID, HEAD, ADDR, THASH, BHASH, SEQ, FEE]) NewTxAttemptWithType(ctx interface{}, tx interface{}, lggr interface{}, txType interface{}, opts ...interface{}) *TxAttemptBuilder_NewTxAttemptWithType_Call[CID, HEAD, ADDR, THASH, BHASH, SEQ, FEE] {
	return &TxAttemptBuilder_NewTxAttemptWithType_Call[CID, HEAD, ADDR, THASH, BHASH, SEQ, FEE]{Call: _e.mock.On("NewTxAttemptWithType",
		append([]interface{}{ctx, tx, lggr, txType}, opts...)...)}
}

func (_c *TxAttemptBuilder_NewTxAttemptWithType_Call[CID, HEAD, ADDR, THASH, BHASH, SEQ, FEE]) Run(run func(ctx context.Context, tx types.Tx[CID, ADDR, THASH, BHASH, SEQ, FEE], lggr logger.Logger, txType int, opts ...fees.Opt)) *TxAttemptBuilder_NewTxAttemptWithType_Call[CID, HEAD, ADDR, THASH, BHASH, SEQ, FEE] {
	_c.Call.Run(func(args mock.Arguments) {
		variadicArgs := make([]fees.Opt, len(args)-4)
		for i, a := range args[4:] {
			if a != nil {
				variadicArgs[i] = a.(fees.Opt)
			}
		}
		run(args[0].(context.Context), args[1].(types.Tx[CID, ADDR, THASH, BHASH, SEQ, FEE]), args[2].(logger.Logger), args[3].(int), variadicArgs...)
	})
	return _c
}

func (_c *TxAttemptBuilder_NewTxAttemptWithType_Call[CID, HEAD, ADDR, THASH, BHASH, SEQ, FEE]) Return(attempt types.TxAttempt[CID, ADDR, THASH, BHASH, SEQ, FEE], fee FEE, feeLimit uint64, retryable bool, err error) *TxAttemptBuilder_NewTxAttemptWithType_Call[CID, HEAD, ADDR, THASH, BHASH, SEQ, FEE] {
	_c.Call.Return(attempt, fee, feeLimit, retryable, err)
	return _c
}

func (_c *TxAttemptBuilder_NewTxAttemptWithType_Call[CID, HEAD, ADDR, THASH, BHASH, SEQ, FEE]) RunAndReturn(run func(context.Context, types.Tx[CID, ADDR, THASH, BHASH, SEQ, FEE], logger.Logger, int, ...fees.Opt) (types.TxAttempt[CID, ADDR, THASH, BHASH, SEQ, FEE], FEE, uint64, bool, error)) *TxAttemptBuilder_NewTxAttemptWithType_Call[CID, HEAD, ADDR, THASH, BHASH, SEQ, FEE] {
	_c.Call.Return(run)
	return _c
}

// OnNewLongestChain provides a mock function with given fields: ctx, head
func (_m *TxAttemptBuilder[CID, HEAD, ADDR, THASH, BHASH, SEQ, FEE]) OnNewLongestChain(ctx context.Context, head HEAD) {
	_m.Called(ctx, head)
}

// TxAttemptBuilder_OnNewLongestChain_Call is a *mock.Call that shadows Run/Return methods with type explicit version for method 'OnNewLongestChain'
type TxAttemptBuilder_OnNewLongestChain_Call[CID chains.ID, HEAD chains.Head[BHASH], ADDR chains.Hashable, THASH chains.Hashable, BHASH chains.Hashable, SEQ chains.Sequence, FEE fees.Fee] struct {
	*mock.Call
}

// OnNewLongestChain is a helper method to define mock.On call
//   - ctx context.Context
//   - head HEAD
func (_e *TxAttemptBuilder_Expecter[CID, HEAD, ADDR, THASH, BHASH, SEQ, FEE]) OnNewLongestChain(ctx interface{}, head interface{}) *TxAttemptBuilder_OnNewLongestChain_Call[CID, HEAD, ADDR, THASH, BHASH, SEQ, FEE] {
	return &TxAttemptBuilder_OnNewLongestChain_Call[CID, HEAD, ADDR, THASH, BHASH, SEQ, FEE]{Call: _e.mock.On("OnNewLongestChain", ctx, head)}
}

func (_c *TxAttemptBuilder_OnNewLongestChain_Call[CID, HEAD, ADDR, THASH, BHASH, SEQ, FEE]) Run(run func(ctx context.Context, head HEAD)) *TxAttemptBuilder_OnNewLongestChain_Call[CID, HEAD, ADDR, THASH, BHASH, SEQ, FEE] {
	_c.Call.Run(func(args mock.Arguments) {
		run(args[0].(context.Context), args[1].(HEAD))
	})
	return _c
}

func (_c *TxAttemptBuilder_OnNewLongestChain_Call[CID, HEAD, ADDR, THASH, BHASH, SEQ, FEE]) Return() *TxAttemptBuilder_OnNewLongestChain_Call[CID, HEAD, ADDR, THASH, BHASH, SEQ, FEE] {
	_c.Call.Return()
	return _c
}

func (_c *TxAttemptBuilder_OnNewLongestChain_Call[CID, HEAD, ADDR, THASH, BHASH, SEQ, FEE]) RunAndReturn(run func(context.Context, HEAD)) *TxAttemptBuilder_OnNewLongestChain_Call[CID, HEAD, ADDR, THASH, BHASH, SEQ, FEE] {
	_c.Run(run)
	return _c
}

// Ready provides a mock function with no fields
func (_m *TxAttemptBuilder[CID, HEAD, ADDR, THASH, BHASH, SEQ, FEE]) Ready() error {
	ret := _m.Called()

	if len(ret) == 0 {
		panic("no return value specified for Ready")
	}

	var r0 error
	if rf, ok := ret.Get(0).(func() error); ok {
		r0 = rf()
	} else {
		r0 = ret.Error(0)
	}

	return r0
}

// TxAttemptBuilder_Ready_Call is a *mock.Call that shadows Run/Return methods with type explicit version for method 'Ready'
type TxAttemptBuilder_Ready_Call[CID chains.ID, HEAD chains.Head[BHASH], ADDR chains.Hashable, THASH chains.Hashable, BHASH chains.Hashable, SEQ chains.Sequence, FEE fees.Fee] struct {
	*mock.Call
}

// Ready is a helper method to define mock.On call
func (_e *TxAttemptBuilder_Expecter[CID, HEAD, ADDR, THASH, BHASH, SEQ, FEE]) Ready() *TxAttemptBuilder_Ready_Call[CID, HEAD, ADDR, THASH, BHASH, SEQ, FEE] {
	return &TxAttemptBuilder_Ready_Call[CID, HEAD, ADDR, THASH, BHASH, SEQ, FEE]{Call: _e.mock.On("Ready")}
}

func (_c *TxAttemptBuilder_Ready_Call[CID, HEAD, ADDR, THASH, BHASH, SEQ, FEE]) Run(run func()) *TxAttemptBuilder_Ready_Call[CID, HEAD, ADDR, THASH, BHASH, SEQ, FEE] {
	_c.Call.Run(func(args mock.Arguments) {
		run()
	})
	return _c
}

func (_c *TxAttemptBuilder_Ready_Call[CID, HEAD, ADDR, THASH, BHASH, SEQ, FEE]) Return(_a0 error) *TxAttemptBuilder_Ready_Call[CID, HEAD, ADDR, THASH, BHASH, SEQ, FEE] {
	_c.Call.Return(_a0)
	return _c
}

func (_c *TxAttemptBuilder_Ready_Call[CID, HEAD, ADDR, THASH, BHASH, SEQ, FEE]) RunAndReturn(run func() error) *TxAttemptBuilder_Ready_Call[CID, HEAD, ADDR, THASH, BHASH, SEQ, FEE] {
	_c.Call.Return(run)
	return _c
}

// Start provides a mock function with given fields: _a0
func (_m *TxAttemptBuilder[CID, HEAD, ADDR, THASH, BHASH, SEQ, FEE]) Start(_a0 context.Context) error {
	ret := _m.Called(_a0)

	if len(ret) == 0 {
		panic("no return value specified for Start")
	}

	var r0 error
	if rf, ok := ret.Get(0).(func(context.Context) error); ok {
		r0 = rf(_a0)
	} else {
		r0 = ret.Error(0)
	}

	return r0
}

// TxAttemptBuilder_Start_Call is a *mock.Call that shadows Run/Return methods with type explicit version for method 'Start'
type TxAttemptBuilder_Start_Call[CID chains.ID, HEAD chains.Head[BHASH], ADDR chains.Hashable, THASH chains.Hashable, BHASH chains.Hashable, SEQ chains.Sequence, FEE fees.Fee] struct {
	*mock.Call
}

// Start is a helper method to define mock.On call
//   - _a0 context.Context
func (_e *TxAttemptBuilder_Expecter[CID, HEAD, ADDR, THASH, BHASH, SEQ, FEE]) Start(_a0 interface{}) *TxAttemptBuilder_Start_Call[CID, HEAD, ADDR, THASH, BHASH, SEQ, FEE] {
	return &TxAttemptBuilder_Start_Call[CID, HEAD, ADDR, THASH, BHASH, SEQ, FEE]{Call: _e.mock.On("Start", _a0)}
}

func (_c *TxAttemptBuilder_Start_Call[CID, HEAD, ADDR, THASH, BHASH, SEQ, FEE]) Run(run func(_a0 context.Context)) *TxAttemptBuilder_Start_Call[CID, HEAD, ADDR, THASH, BHASH, SEQ, FEE] {
	_c.Call.Run(func(args mock.Arguments) {
		run(args[0].(context.Context))
	})
	return _c
}

func (_c *TxAttemptBuilder_Start_Call[CID, HEAD, ADDR, THASH, BHASH, SEQ, FEE]) Return(_a0 error) *TxAttemptBuilder_Start_Call[CID, HEAD, ADDR, THASH, BHASH, SEQ, FEE] {
	_c.Call.Return(_a0)
	return _c
}

func (_c *TxAttemptBuilder_Start_Call[CID, HEAD, ADDR, THASH, BHASH, SEQ, FEE]) RunAndReturn(run func(context.Context) error) *TxAttemptBuilder_Start_Call[CID, HEAD, ADDR, THASH, BHASH, SEQ, FEE] {
	_c.Call.Return(run)
	return _c
}

// NewTxAttemptBuilder creates a new instance of TxAttemptBuilder. It also registers a testing interface on the mock and a cleanup function to assert the mocks expectations.
// The first argument is typically a *testing.T value.
func NewTxAttemptBuilder[CID chains.ID, HEAD chains.Head[BHASH], ADDR chains.Hashable, THASH chains.Hashable, BHASH chains.Hashable, SEQ chains.Sequence, FEE fees.Fee](t interface {
	mock.TestingT
	Cleanup(func())
}) *TxAttemptBuilder[CID, HEAD, ADDR, THASH, BHASH, SEQ, FEE] {
	mock := &TxAttemptBuilder[CID, HEAD, ADDR, THASH, BHASH, SEQ, FEE]{}
	mock.Mock.Test(t)

	t.Cleanup(func() { mock.AssertExpectations(t) })

	return mock
}
