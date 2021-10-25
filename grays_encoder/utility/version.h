/*------------------------------------------------------------------------------
	()      File:   version.h
	/\      Copyright (c) 2021 Andrew Woodward-May
   //\\
  //  \\    Description:
				App Version
				Just hard coded for now, these should probably be in the build
				system though.
------------------------------------------------------------------------------*/
#pragma once
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
enum class AppDeployType { Stable, ReleaseCandidate, Development };

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
#define DEPLOY_TYPE AppDeployType::Development
#define MAJOR_VERSION 2
#define MINOR_VERSION 0
#define ADDITIONAL_VERSION 0

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
#define COPYRIGHT_YEARS_BEGIN 2021
#define COPYRIGHT_YEARS_END 2021
#define COPYRIGHT_HOLDERS "Andrew Woodward-May"
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------

#define INTERNAL_STRINGIFY(x) INTERNAL_MAKE_RC_RESOLVE_STRING(x)
#define INTERNAL_MAKE_RC_RESOLVE_STRING(x) #x

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
constexpr const char* GetVersionString()
{
	switch ( DEPLOY_TYPE )
	{
	case AppDeployType::Development:
		return "Version " INTERNAL_STRINGIFY(MAJOR_VERSION) "." INTERNAL_STRINGIFY(MINOR_VERSION) "(dev)";
	case AppDeployType::ReleaseCandidate:
		return "Version " INTERNAL_STRINGIFY( MAJOR_VERSION ) "." INTERNAL_STRINGIFY( MINOR_VERSION ) "(rc" INTERNAL_STRINGIFY( ADDITIONAL_VERSION) ")";
	}

	return "Version " INTERNAL_STRINGIFY( MAJOR_VERSION ) "." INTERNAL_STRINGIFY( MINOR_VERSION );
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
constexpr const char8_t* GetCopyrightString()
{
	if( COPYRIGHT_YEARS_BEGIN == COPYRIGHT_YEARS_END )
	{
		return u8"Copyright © " INTERNAL_STRINGIFY(COPYRIGHT_YEARS_BEGIN) " " COPYRIGHT_HOLDERS;
	}
	else
	{
		return u8"Copyright © " INTERNAL_STRINGIFY( COPYRIGHT_YEARS_BEGIN ) "-" INTERNAL_STRINGIFY( COPYRIGHT_YEARS_END ) " " COPYRIGHT_HOLDERS;
	}
}


//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
#define APP_VERSION GetVersionString()
#define APP_COPYRIGHT GetCopyrightString()