import { Text, Widget } from "@lcui/react";
import CodeBlockCopy from "../../../components/code-block-copy";

export default function AboutDocPage() {
  return (
    <Widget className="doc-page">
      <h1>About LCUI</h1>
      <p className="doc-page-desc">{`LCUI is an open-source desktop graphical user interface library written in C, designed to provide C developers with a simple and easy-to-use GUI development experience, while incorporating CSS styling and declarative UI description from web development to lower the learning barrier.`}</p>
      <Widget className="doc-section">
        <h2>Key features</h2>
        <ul>
          <li><Text>{`[b]Cross-platform[/b] — Supports Windows and Linux.`}</Text></li>
          <li><Text>{`[b]Fully self-drawn widgets[/b] — Widgets maintain consistent appearance and behavior across platforms.`}</Text></li>
          <li><Text>{`[b]DPI-adaptive[/b] — Automatically scales the UI on high-resolution screens for crisp display.`}</Text></li>
          <li><Text>{`[b]Built-in CSS engine[/b] — Supports using CSS to define UI styles and layouts, making it easy to pick up for those with web development experience.`}</Text></li>
          <li><Text>{`[b]Modern development tools[/b] — Through the [bgcolor=#eee] @lcui/cli [/bgcolor] tool, you can use TypeScript with JSX syntax to write user interfaces.`}</Text></li>
        </ul>
      </Widget>
      <Widget className="doc-section">
        <h2>Who should use LCUI</h2>
        <p>{`LCUI is suitable for these developers:`}</p>
        <ul>
          <li><Text>{`Want to keep using C on the desktop, but want to escape the tedious Win32 / X11 native development experience.`}</Text></li>
          <li><Text>{`Already familiar with web frontend (HTML, CSS) and want to transfer that experience to desktop applications.`}</Text></li>
          <li><Text>{`Need to build single-window desktop tools with simple UI content.`}</Text></li>
        </ul>
        <p>{`If your use case is a large commercial desktop product, a game engine, or a tool deeply integrated with the OS, LCUI may not be the best choice; in such scenarios, consider Qt, GTK, or native APIs instead.`}</p>
      </Widget>
      <Widget className="doc-section">
        <h2>Architecture</h2>
        <p>{`LCUI is divided into four layers from top to bottom:`}</p>
        <h3>Application layer</h3>
        <p>{`Your business code: custom widgets, CSS styles, TSX/JSX code, event handling. This is the only part tied to your project domain.`}</p>
        <h3>LCUI Runtime</h3>
        <p>{`Initialization, event loop, application lifecycle management. Responsible for running the app, dispatching events, and driving rendering.`}</p>
        <h3>UI Helpers</h3>
        <p>{`UI XML parsing, UI Router, cursor management, internationalization (i18n), and other helper modules. These package common UI behaviors into pluggable subsystems.`}</p>
        <h3>Foundation layer</h3>
        <p>{`YUtil (general utility library), PandaGL (2D rendering engine), CSS engine, UI widget system, Thread / Worker abstractions. These modules can also be used independently.`}</p>
        <p>{`The platform layer (Windows / Linux) handles window management and input events.`}</p>
      </Widget>
      <Widget className="doc-section">
        <h2>License</h2>
        <p>{`LCUI is released under the MIT License. See LICENSE.TXT in the repository root for details.`}</p>
      </Widget>
      <Widget className="doc-section">
        <h2>Contributing</h2>
        <p>{`Contributions are welcome! Before submitting a Pull Request, please read CONTRIBUTING.md in the repository root.`}</p>
        <ul>
          <li><Text>{`[b]Bug reports[/b] — Open an issue on GitHub Issues.`}</Text></li>
          <li><Text>{`[b]Feature requests[/b] — Start a discussion on GitHub Discussions.`}</Text></li>
          <li><Text>{`[b]Code contributions[/b] — Fork the repo, create a branch, and submit a PR.`}</Text></li>
        </ul>
      </Widget>
      <Widget className="doc-section">
        <h2>Community</h2>
        <p>{`If you run into issues, you can ask questions on GitHub Discussions (use the [bgcolor=#eee] Q&A [/bgcolor] label). We also encourage experienced users to help newcomers.`}</p>
      </Widget>
    </Widget>
  );
}
