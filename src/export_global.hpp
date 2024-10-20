#pragma once
#ifndef _EXPORT_GLOBAL_HPP_
#define _EXPORT_GLOBAL_HPP_
#if defined(TRT_LIB)
#define TRT_EXPORT __declspec(dllexport)
#else
#define TRT_EXPORT __declspec(dllimport)
#endif
#endif