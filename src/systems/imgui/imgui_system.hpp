#pragma once

class Context;

class ImGuiSystem {
  public:
	ImGuiSystem(Context& context);

  private:
	Context& _context;
};