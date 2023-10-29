#include "pch.hpp"

#include <cache/entities/hen.hpp>

#include <renderer/renderer.hpp>
#include <vars/vars.hpp>

#include <bindings/baseentity.hpp>

bool CCachedHen::CanDrawESP() { return Get() && g_Vars.m_ChickenESP; }
