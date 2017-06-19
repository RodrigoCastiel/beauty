//  +-----------------------------------------------------+
//	|	File: BModel.h									  |					
//	|	Author: Rodrigo Castiel							  |
//	|	Date: June 18, 2017.						      |
//	+-----------------------------------------------------+
#pragma once

#include <parsing/RenderingConfig.h>

class BModel
{
public:
	// === Contructors/Destructor ===
	BModel();
	~BModel();

	// === Main Methods ===

	// === Setters / Getters ===

	void SetRenderingConfig(const renderer::RenderingConfig & config);
	const renderer::RenderingConfig & GetRenderingConfig() const;

private:
	renderer::RenderingConfig mRenderingConfig;

};