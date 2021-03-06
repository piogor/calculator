// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#include "pch.h"
#include "GraphingSettingsViewModel.h"
#include <CalcManager\NumberFormattingUtils.h>

using namespace CalculatorApp::ViewModel;
using namespace CalcManager::NumberFormattingUtils;
using namespace GraphControl;
using namespace std;
using namespace Platform;

GraphingSettingsViewModel::GraphingSettingsViewModel()
    : m_XMinValue(0)
    , m_XMaxValue(0)
    , m_YMinValue(0)
    , m_YMaxValue(0)
    , m_XMinError(false)
    , m_XMaxError(false)
    , m_YMinError(false)
    , m_YMaxError(false)
    , m_dontUpdateDisplayRange()
    , m_XIsMinLastChanged(true)
    , m_YIsMinLastChanged(true)
{
}

void GraphingSettingsViewModel::SetGrapher(Grapher ^ grapher)
{
    if (grapher != nullptr)
    {
        if (grapher->TrigUnitMode == (int)Graphing::EvalTrigUnitMode::Invalid)
        {
            grapher->TrigUnitMode = (int)Graphing::EvalTrigUnitMode::Radians;
        }
    }
    Graph = grapher;
    InitRanges();
    RaisePropertyChanged(L"TrigUnit");
}

void GraphingSettingsViewModel::InitRanges()
{
    double xMin = 0, xMax = 0, yMin = 0, yMax = 0;
    if (m_Graph != nullptr)
    {
        m_Graph->GetDisplayRanges(&xMin, &xMax, &yMin, &yMax);
    }
    m_dontUpdateDisplayRange = true;
    m_XMinValue = xMin;
    m_XMaxValue = xMax;
    m_YMinValue = yMin;
    m_YMaxValue = yMax;
    auto valueStr = to_wstring(m_XMinValue);
    TrimTrailingZeros(valueStr);
    m_XMin = ref new String(valueStr.c_str());

    valueStr = to_wstring(m_XMaxValue);
    TrimTrailingZeros(valueStr);
    m_XMax = ref new String(valueStr.c_str());

    valueStr = to_wstring(m_YMinValue);
    TrimTrailingZeros(valueStr);
    m_YMin = ref new String(valueStr.c_str());

    valueStr = to_wstring(m_YMaxValue);
    TrimTrailingZeros(valueStr);
    m_YMax = ref new String(valueStr.c_str());

    m_dontUpdateDisplayRange = false;
}

void GraphingSettingsViewModel::UpdateDisplayRange()
{
    if (m_Graph == nullptr || m_dontUpdateDisplayRange || HasError())
    {
        return;
    }

    m_Graph->SetDisplayRanges(m_XMinValue, m_XMaxValue, m_YMinValue, m_YMaxValue);
}

bool GraphingSettingsViewModel::HasError()
{
    return m_XMinError || m_YMinError || m_XMaxError || m_YMaxError || XError || YError;
}
