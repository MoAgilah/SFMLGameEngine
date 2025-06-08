STEP 1: Open Properties for Game Project
Right-click Game in Solution Explorer

Click Properties

Make sure you’re editing for:

Configuration: All Configurations or both Debug and Release

Platform: x64

🔹 STEP 2: Set Library Directories
To find .lib files like sfml-graphics-d.lib:

Go to:

mathematica
Copy
Edit
Configuration Properties → VC++ Directories → Library Directories
Set the value to:

text
Copy
Edit
$(SolutionDir)SFMLGameEngine\SFMLGameEngine\lib;%(AdditionalLibraryDirectories)
🔹 STEP 3: Set Executable Directories
To find DLLs like sfml-graphics-d-3.dll:

Go to:

mathematica
Copy
Edit
Configuration Properties → VC++ Directories → Executable Directories
Set the value to:

text
Copy
Edit
$(SolutionDir)SFMLGameEngine\SFMLGameEngine\bin\$(Configuration);%(ExecutableDirectories)
✅ This ensures that:

When you run Game.exe, it can find sfml-*.dll in the right place

It still includes paths to MSVC tools like cl.exe (because of %(ExecutableDirectories))

🔹 STEP 4: Set Additional Dependencies
Go to:

mathematica
Copy
Edit
Configuration Properties → Linker → Input → Additional Dependencies
Add this (for Debug configuration):

text
Copy
Edit
sfml-graphics-d.lib;sfml-window-d.lib;sfml-system-d.lib;sfml-audio-d.lib;sfml-network-d.lib;%(AdditionalDependencies)
For Release, use the non--d versions:

text
Copy
Edit
sfml-graphics.lib;sfml-window.lib;sfml-system.lib;sfml-audio.lib;sfml-network.lib;%(AdditionalDependencies)
🔹 STEP 5: Confirm SFMLGameEngine Doesn't Link SFML
In the SFMLGameEngine project:

Right-click → Properties

Go to:

css
Copy
Edit
Linker → Input → Additional Dependencies
❌ Make sure SFML .lib files are NOT listed here
They should only be in the Game project.

Under:

Copy
Edit
VC++ Directories → Library Directories
❌ Remove any entry like $(ProjectDir)lib unless you’re doing something custom