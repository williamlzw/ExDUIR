// Copyright (C) Microsoft Corporation. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#pragma once

#include <objbase.h>
#include <wrl/implements.h>

#include "WebView2EnvironmentOptions.h"
#include "webview2experimental.h"

// This is a base COM class that implements IUnknown if there is no experimental
// options, or ICoreWebView2ExperimentalEnvironmentOptions when there are
// experimental options.
template <typename allocate_fn_t,
          allocate_fn_t allocate_fn,
          typename deallocate_fn_t,
          deallocate_fn_t deallocate_fn>
class CoreWebView2ExperimentalEnvironmentOptionsBase
    : public Microsoft::WRL::Implements<
          Microsoft::WRL::RuntimeClassFlags<Microsoft::WRL::ClassicCom>,
          IUnknown> {
 public:
  CoreWebView2ExperimentalEnvironmentOptionsBase() {}

 protected:
  ~CoreWebView2ExperimentalEnvironmentOptionsBase(){};

};

template <typename allocate_fn_t,
          allocate_fn_t allocate_fn,
          typename deallocate_fn_t,
          deallocate_fn_t deallocate_fn>
class CoreWebView2ExperimentalEnvironmentOptionsClass
    : public Microsoft::WRL::RuntimeClass<
          Microsoft::WRL::RuntimeClassFlags<Microsoft::WRL::ClassicCom>,
          CoreWebView2EnvironmentOptionsBase<allocate_fn_t,
                                             allocate_fn,
                                             deallocate_fn_t,
                                             deallocate_fn>,
          CoreWebView2ExperimentalEnvironmentOptionsBase<allocate_fn_t,
                                                         allocate_fn,
                                                         deallocate_fn_t,
                                                         deallocate_fn>> {
 public:
  CoreWebView2ExperimentalEnvironmentOptionsClass() {}

 protected:
  ~CoreWebView2ExperimentalEnvironmentOptionsClass() override{};
};

typedef CoreWebView2ExperimentalEnvironmentOptionsClass<
    decltype(&::CoTaskMemAlloc),
    ::CoTaskMemAlloc,
    decltype(&::CoTaskMemFree),
    ::CoTaskMemFree>
    CoreWebView2ExperimentalEnvironmentOptions;


