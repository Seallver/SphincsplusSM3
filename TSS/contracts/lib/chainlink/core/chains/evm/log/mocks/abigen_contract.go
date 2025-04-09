// Code generated by mockery v2.53.0. DO NOT EDIT.

package mocks

import (
	common "github.com/ethereum/go-ethereum/common"
	generated "github.com/smartcontractkit/chainlink-evm/gethwrappers/generated"

	mock "github.com/stretchr/testify/mock"

	types "github.com/ethereum/go-ethereum/core/types"
)

// AbigenContract is an autogenerated mock type for the AbigenContract type
type AbigenContract struct {
	mock.Mock
}

type AbigenContract_Expecter struct {
	mock *mock.Mock
}

func (_m *AbigenContract) EXPECT() *AbigenContract_Expecter {
	return &AbigenContract_Expecter{mock: &_m.Mock}
}

// Address provides a mock function with no fields
func (_m *AbigenContract) Address() common.Address {
	ret := _m.Called()

	if len(ret) == 0 {
		panic("no return value specified for Address")
	}

	var r0 common.Address
	if rf, ok := ret.Get(0).(func() common.Address); ok {
		r0 = rf()
	} else {
		if ret.Get(0) != nil {
			r0 = ret.Get(0).(common.Address)
		}
	}

	return r0
}

// AbigenContract_Address_Call is a *mock.Call that shadows Run/Return methods with type explicit version for method 'Address'
type AbigenContract_Address_Call struct {
	*mock.Call
}

// Address is a helper method to define mock.On call
func (_e *AbigenContract_Expecter) Address() *AbigenContract_Address_Call {
	return &AbigenContract_Address_Call{Call: _e.mock.On("Address")}
}

func (_c *AbigenContract_Address_Call) Run(run func()) *AbigenContract_Address_Call {
	_c.Call.Run(func(args mock.Arguments) {
		run()
	})
	return _c
}

func (_c *AbigenContract_Address_Call) Return(_a0 common.Address) *AbigenContract_Address_Call {
	_c.Call.Return(_a0)
	return _c
}

func (_c *AbigenContract_Address_Call) RunAndReturn(run func() common.Address) *AbigenContract_Address_Call {
	_c.Call.Return(run)
	return _c
}

// ParseLog provides a mock function with given fields: _a0
func (_m *AbigenContract) ParseLog(_a0 types.Log) (generated.AbigenLog, error) {
	ret := _m.Called(_a0)

	if len(ret) == 0 {
		panic("no return value specified for ParseLog")
	}

	var r0 generated.AbigenLog
	var r1 error
	if rf, ok := ret.Get(0).(func(types.Log) (generated.AbigenLog, error)); ok {
		return rf(_a0)
	}
	if rf, ok := ret.Get(0).(func(types.Log) generated.AbigenLog); ok {
		r0 = rf(_a0)
	} else {
		if ret.Get(0) != nil {
			r0 = ret.Get(0).(generated.AbigenLog)
		}
	}

	if rf, ok := ret.Get(1).(func(types.Log) error); ok {
		r1 = rf(_a0)
	} else {
		r1 = ret.Error(1)
	}

	return r0, r1
}

// AbigenContract_ParseLog_Call is a *mock.Call that shadows Run/Return methods with type explicit version for method 'ParseLog'
type AbigenContract_ParseLog_Call struct {
	*mock.Call
}

// ParseLog is a helper method to define mock.On call
//   - _a0 types.Log
func (_e *AbigenContract_Expecter) ParseLog(_a0 interface{}) *AbigenContract_ParseLog_Call {
	return &AbigenContract_ParseLog_Call{Call: _e.mock.On("ParseLog", _a0)}
}

func (_c *AbigenContract_ParseLog_Call) Run(run func(_a0 types.Log)) *AbigenContract_ParseLog_Call {
	_c.Call.Run(func(args mock.Arguments) {
		run(args[0].(types.Log))
	})
	return _c
}

func (_c *AbigenContract_ParseLog_Call) Return(_a0 generated.AbigenLog, _a1 error) *AbigenContract_ParseLog_Call {
	_c.Call.Return(_a0, _a1)
	return _c
}

func (_c *AbigenContract_ParseLog_Call) RunAndReturn(run func(types.Log) (generated.AbigenLog, error)) *AbigenContract_ParseLog_Call {
	_c.Call.Return(run)
	return _c
}

// NewAbigenContract creates a new instance of AbigenContract. It also registers a testing interface on the mock and a cleanup function to assert the mocks expectations.
// The first argument is typically a *testing.T value.
func NewAbigenContract(t interface {
	mock.TestingT
	Cleanup(func())
}) *AbigenContract {
	mock := &AbigenContract{}
	mock.Mock.Test(t)

	t.Cleanup(func() { mock.AssertExpectations(t) })

	return mock
}
