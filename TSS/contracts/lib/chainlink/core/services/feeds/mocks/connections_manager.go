// Code generated by mockery v2.53.0. DO NOT EDIT.

package mocks

import (
	feedsmanager "github.com/smartcontractkit/chainlink-protos/orchestrator/feedsmanager"
	feeds "github.com/smartcontractkit/chainlink/v2/core/services/feeds"

	mock "github.com/stretchr/testify/mock"
)

// ConnectionsManager is an autogenerated mock type for the ConnectionsManager type
type ConnectionsManager struct {
	mock.Mock
}

type ConnectionsManager_Expecter struct {
	mock *mock.Mock
}

func (_m *ConnectionsManager) EXPECT() *ConnectionsManager_Expecter {
	return &ConnectionsManager_Expecter{mock: &_m.Mock}
}

// Close provides a mock function with no fields
func (_m *ConnectionsManager) Close() {
	_m.Called()
}

// ConnectionsManager_Close_Call is a *mock.Call that shadows Run/Return methods with type explicit version for method 'Close'
type ConnectionsManager_Close_Call struct {
	*mock.Call
}

// Close is a helper method to define mock.On call
func (_e *ConnectionsManager_Expecter) Close() *ConnectionsManager_Close_Call {
	return &ConnectionsManager_Close_Call{Call: _e.mock.On("Close")}
}

func (_c *ConnectionsManager_Close_Call) Run(run func()) *ConnectionsManager_Close_Call {
	_c.Call.Run(func(args mock.Arguments) {
		run()
	})
	return _c
}

func (_c *ConnectionsManager_Close_Call) Return() *ConnectionsManager_Close_Call {
	_c.Call.Return()
	return _c
}

func (_c *ConnectionsManager_Close_Call) RunAndReturn(run func()) *ConnectionsManager_Close_Call {
	_c.Run(run)
	return _c
}

// Connect provides a mock function with given fields: opts
func (_m *ConnectionsManager) Connect(opts feeds.ConnectOpts) {
	_m.Called(opts)
}

// ConnectionsManager_Connect_Call is a *mock.Call that shadows Run/Return methods with type explicit version for method 'Connect'
type ConnectionsManager_Connect_Call struct {
	*mock.Call
}

// Connect is a helper method to define mock.On call
//   - opts feeds.ConnectOpts
func (_e *ConnectionsManager_Expecter) Connect(opts interface{}) *ConnectionsManager_Connect_Call {
	return &ConnectionsManager_Connect_Call{Call: _e.mock.On("Connect", opts)}
}

func (_c *ConnectionsManager_Connect_Call) Run(run func(opts feeds.ConnectOpts)) *ConnectionsManager_Connect_Call {
	_c.Call.Run(func(args mock.Arguments) {
		run(args[0].(feeds.ConnectOpts))
	})
	return _c
}

func (_c *ConnectionsManager_Connect_Call) Return() *ConnectionsManager_Connect_Call {
	_c.Call.Return()
	return _c
}

func (_c *ConnectionsManager_Connect_Call) RunAndReturn(run func(feeds.ConnectOpts)) *ConnectionsManager_Connect_Call {
	_c.Run(run)
	return _c
}

// Disconnect provides a mock function with given fields: id
func (_m *ConnectionsManager) Disconnect(id int64) error {
	ret := _m.Called(id)

	if len(ret) == 0 {
		panic("no return value specified for Disconnect")
	}

	var r0 error
	if rf, ok := ret.Get(0).(func(int64) error); ok {
		r0 = rf(id)
	} else {
		r0 = ret.Error(0)
	}

	return r0
}

// ConnectionsManager_Disconnect_Call is a *mock.Call that shadows Run/Return methods with type explicit version for method 'Disconnect'
type ConnectionsManager_Disconnect_Call struct {
	*mock.Call
}

// Disconnect is a helper method to define mock.On call
//   - id int64
func (_e *ConnectionsManager_Expecter) Disconnect(id interface{}) *ConnectionsManager_Disconnect_Call {
	return &ConnectionsManager_Disconnect_Call{Call: _e.mock.On("Disconnect", id)}
}

func (_c *ConnectionsManager_Disconnect_Call) Run(run func(id int64)) *ConnectionsManager_Disconnect_Call {
	_c.Call.Run(func(args mock.Arguments) {
		run(args[0].(int64))
	})
	return _c
}

func (_c *ConnectionsManager_Disconnect_Call) Return(_a0 error) *ConnectionsManager_Disconnect_Call {
	_c.Call.Return(_a0)
	return _c
}

func (_c *ConnectionsManager_Disconnect_Call) RunAndReturn(run func(int64) error) *ConnectionsManager_Disconnect_Call {
	_c.Call.Return(run)
	return _c
}

// GetClient provides a mock function with given fields: id
func (_m *ConnectionsManager) GetClient(id int64) (feedsmanager.FeedsManagerClient, error) {
	ret := _m.Called(id)

	if len(ret) == 0 {
		panic("no return value specified for GetClient")
	}

	var r0 feedsmanager.FeedsManagerClient
	var r1 error
	if rf, ok := ret.Get(0).(func(int64) (feedsmanager.FeedsManagerClient, error)); ok {
		return rf(id)
	}
	if rf, ok := ret.Get(0).(func(int64) feedsmanager.FeedsManagerClient); ok {
		r0 = rf(id)
	} else {
		if ret.Get(0) != nil {
			r0 = ret.Get(0).(feedsmanager.FeedsManagerClient)
		}
	}

	if rf, ok := ret.Get(1).(func(int64) error); ok {
		r1 = rf(id)
	} else {
		r1 = ret.Error(1)
	}

	return r0, r1
}

// ConnectionsManager_GetClient_Call is a *mock.Call that shadows Run/Return methods with type explicit version for method 'GetClient'
type ConnectionsManager_GetClient_Call struct {
	*mock.Call
}

// GetClient is a helper method to define mock.On call
//   - id int64
func (_e *ConnectionsManager_Expecter) GetClient(id interface{}) *ConnectionsManager_GetClient_Call {
	return &ConnectionsManager_GetClient_Call{Call: _e.mock.On("GetClient", id)}
}

func (_c *ConnectionsManager_GetClient_Call) Run(run func(id int64)) *ConnectionsManager_GetClient_Call {
	_c.Call.Run(func(args mock.Arguments) {
		run(args[0].(int64))
	})
	return _c
}

func (_c *ConnectionsManager_GetClient_Call) Return(_a0 feedsmanager.FeedsManagerClient, _a1 error) *ConnectionsManager_GetClient_Call {
	_c.Call.Return(_a0, _a1)
	return _c
}

func (_c *ConnectionsManager_GetClient_Call) RunAndReturn(run func(int64) (feedsmanager.FeedsManagerClient, error)) *ConnectionsManager_GetClient_Call {
	_c.Call.Return(run)
	return _c
}

// IsConnected provides a mock function with given fields: id
func (_m *ConnectionsManager) IsConnected(id int64) bool {
	ret := _m.Called(id)

	if len(ret) == 0 {
		panic("no return value specified for IsConnected")
	}

	var r0 bool
	if rf, ok := ret.Get(0).(func(int64) bool); ok {
		r0 = rf(id)
	} else {
		r0 = ret.Get(0).(bool)
	}

	return r0
}

// ConnectionsManager_IsConnected_Call is a *mock.Call that shadows Run/Return methods with type explicit version for method 'IsConnected'
type ConnectionsManager_IsConnected_Call struct {
	*mock.Call
}

// IsConnected is a helper method to define mock.On call
//   - id int64
func (_e *ConnectionsManager_Expecter) IsConnected(id interface{}) *ConnectionsManager_IsConnected_Call {
	return &ConnectionsManager_IsConnected_Call{Call: _e.mock.On("IsConnected", id)}
}

func (_c *ConnectionsManager_IsConnected_Call) Run(run func(id int64)) *ConnectionsManager_IsConnected_Call {
	_c.Call.Run(func(args mock.Arguments) {
		run(args[0].(int64))
	})
	return _c
}

func (_c *ConnectionsManager_IsConnected_Call) Return(_a0 bool) *ConnectionsManager_IsConnected_Call {
	_c.Call.Return(_a0)
	return _c
}

func (_c *ConnectionsManager_IsConnected_Call) RunAndReturn(run func(int64) bool) *ConnectionsManager_IsConnected_Call {
	_c.Call.Return(run)
	return _c
}

// NewConnectionsManager creates a new instance of ConnectionsManager. It also registers a testing interface on the mock and a cleanup function to assert the mocks expectations.
// The first argument is typically a *testing.T value.
func NewConnectionsManager(t interface {
	mock.TestingT
	Cleanup(func())
}) *ConnectionsManager {
	mock := &ConnectionsManager{}
	mock.Mock.Test(t)

	t.Cleanup(func() { mock.AssertExpectations(t) })

	return mock
}
