import React from "react";
import { render } from "@testing-library/react";
import { describe, it, expect } from "vitest";
import MovingBG from "../components/MovingBG";

describe("MovingBG Component", () => {
  it("renders without crashing", () => {
    const { container } = render(<MovingBG />);
    expect(container).toBeTruthy();
  });

  it("renders a list with 36 list items", () => {
    const { container } = render(<MovingBG />);
    const listItems = container.querySelectorAll("ul.background li");
    expect(listItems.length).toBe(36);
  });

  it('has a ul element with class "background"', () => {
    const { container } = render(<MovingBG />);
    const ulElement = container.querySelector("ul.background");
    expect(ulElement).toBeTruthy();
  });
});
