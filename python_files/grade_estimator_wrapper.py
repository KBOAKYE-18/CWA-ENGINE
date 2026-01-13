import ctypes
from typing import Optional, Tuple
from pathlib import Path


class GradeEstimatorError(Exception):
    """Custom exception for Grade Estimator errors"""
    pass


class GradeEstimator:
    """
    Python wrapper for the Grade Estimator C Engine.
    
    This class provides a safe interface to the C library, handling:
    - Library loading
    - Object lifecycle management
    - Input validation
    - CWA/CGPA calculation workflows
    """
    
    def __init__(self, library_path: str):
        """
        Initialize the Grade Estimator wrapper.
        
        Args:
            library_path: Path to the compiled C library (.so on Linux, .dll on Windows, .dylib on macOS)
        """
        try:
            self.lib = ctypes.CDLL(library_path)
        except OSError as e:
            raise GradeEstimatorError(f"Failed to load library: {e}")
        
        self._setup_function_signatures()
        self._student_obj = None
        self._current_mode = None  # Track whether we're using CWA or CGPA
    
    def _setup_function_signatures(self):
        """
        Define the C function signatures using ctypes.
        This tells Python what types the C functions expect and return.
        
        Based on the actual C API:
        Student init_student(int credits_com, int credits_remain);
        float calculate_dist_CWA(const Student student, float target_cwa, float current_cwa);
        float calculate_dist_CGPA(const Student student, float target_cgpa, float current_cgpa);
        float recalculate_dist_CWA(const Student student, float total_achievable_WA, int total_achievable_Credits);
        float recalculate_dist_CGPA(const Student student, float total_achievable_gradePts, int total_achievable_Credits);
        void destroy_object(const Student student);
        """
        # init_student: creates and returns a Student object (likely a struct or pointer)
        # We'll treat Student as c_void_p (generic pointer) since we don't know its internal structure
        self.lib.init_student.restype = ctypes.c_void_p
        self.lib.init_student.argtypes = [
            ctypes.c_int,  # credits_com (completed credits)
            ctypes.c_int   # credits_remain (remaining credits)
        ]
        
        # destroy_object: frees memory for a student object
        self.lib.destroy_object.restype = None
        self.lib.destroy_object.argtypes = [ctypes.c_void_p]
        
        # calculate_dist_CWA: returns float
        self.lib.calculate_dist_CWA.restype = ctypes.c_float
        self.lib.calculate_dist_CWA.argtypes = [
            ctypes.c_void_p,  # Student object
            ctypes.c_float,   # target_cwa
            ctypes.c_float    # current_cwa
        ]
        
        # recalculate_dist_CWA: returns float
        self.lib.recalculate_dist_CWA.restype = ctypes.c_float
        self.lib.recalculate_dist_CWA.argtypes = [
            ctypes.c_void_p,  # Student object
            ctypes.c_float,   # total_achievable_WA
            ctypes.c_int      # total_achievable_Credits
        ]
        
        # calculate_dist_CGPA: returns float
        self.lib.calculate_dist_CGPA.restype = ctypes.c_float
        self.lib.calculate_dist_CGPA.argtypes = [
            ctypes.c_void_p,  # Student object
            ctypes.c_float,   # target_cgpa
            ctypes.c_float    # current_cgpa
        ]
        
        # recalculate_dist_CGPA: returns float
        self.lib.recalculate_dist_CGPA.restype = ctypes.c_float
        self.lib.recalculate_dist_CGPA.argtypes = [
            ctypes.c_void_p,  # Student object
            ctypes.c_float,   # total_achievable_gradePts
            ctypes.c_int      # total_achievable_Credits
        ]
    
    def _init_student_object(self, credits_completed: int, credits_remaining: int):
        """
        Create a new student object in the C engine.
        
        Args:
            credits_completed: Number of credits already completed
            credits_remaining: Number of credits left to complete
        """
        if self._student_obj is not None:
            self._destroy_student_object()
        
        self._student_obj = self.lib.init_student(
            ctypes.c_int(credits_completed),
            ctypes.c_int(credits_remaining)
        )
        
        if self._student_obj is None or self._student_obj == 0:
            raise GradeEstimatorError("Failed to initialize student object")
    
    def _destroy_student_object(self):
        """Safely destroy the current student object"""
        if self._student_obj is not None:
            self.lib.destroy_object(self._student_obj)
            self._student_obj = None
            self._current_mode = None
    
    def _validate_cwa_inputs(self, current_cwa: float, target_cwa: float):
        """Validate inputs for CWA calculations"""
        if not (0.0 <= current_cwa <= 100.0):
            raise ValueError("Current CWA must be between 0 and 100")
        if not (0.0 <= target_cwa <= 100.0):
            raise ValueError("Target CWA must be between 0 and 100")
        if target_cwa < current_cwa:
            raise ValueError("Target CWA must be greater than or equal to current CWA")
    
    def _validate_cgpa_inputs(self, current_cgpa: float, target_cgpa: float):
        """Validate inputs for CGPA calculations"""
        if not (0.0 <= current_cgpa <= 4.0):
            raise ValueError("Current CGPA must be between 0 and 4.0")
        if not (0.0 <= target_cgpa <= 4.0):
            raise ValueError("Target CGPA must be between 0 and 4.0")
        if target_cgpa < current_cgpa:
            raise ValueError("Target CGPA must be greater than or equal to current CGPA")
    
    def _validate_credits(self, credits: int, name: str = "Credits"):
        """Validate credit values"""
        if credits < 0:
            raise ValueError(f"{name} cannot be negative")
    
    def calculate_cwa(self, current_cwa: float, target_cwa: float,
                     credits_completed: int, credits_remaining: int) -> float:
        """
        Calculate the required CWA distribution.
        
        Args:
            current_cwa: Student's current cumulative weighted average (0-100)
            target_cwa: Desired target cumulative weighted average (0-100)
            credits_completed: Number of credits already completed
            credits_remaining: Number of credits left to complete
            
        Returns:
            Required CWA per remaining course
            
        Raises:
            ValueError: If inputs are invalid
            GradeEstimatorError: If calculation fails
        """
        self._validate_cwa_inputs(current_cwa, target_cwa)
        self._validate_credits(credits_completed, "Credits completed")
        self._validate_credits(credits_remaining, "Credits remaining")
        
        # Check for mode conflict
        if self._current_mode == "CGPA":
            raise GradeEstimatorError("Cannot mix CWA and CGPA calculations. Create a new instance.")
        
        # Initialize fresh student object for new calculation
        self._init_student_object(credits_completed, credits_remaining)
        self._current_mode = "CWA"
        
        try:
            result = self.lib.calculate_dist_CWA(
                self._student_obj,
                ctypes.c_float(target_cwa),
                ctypes.c_float(current_cwa)
            )
            return float(result)
        except Exception as e:
            self._destroy_student_object()
            raise GradeEstimatorError(f"CWA calculation failed: {e}")
    
    def recalculate_cwa(self, total_achievable_wa: float, total_achievable_credits: int) -> float:
        """
        Recalculate CWA requirements after locking additional courses.
        
        Must be called after calculate_cwa() on the same instance.
        
        Args:
            total_achievable_wa: Total achievable weighted average for locked courses
            total_achievable_credits: Sum of credit hours for locked courses
            
        Returns:
            Updated required CWA per remaining course
            
        Raises:
            ValueError: If inputs are invalid
            GradeEstimatorError: If calculation fails or called before calculate_cwa()
        """
        if self._student_obj is None or self._current_mode != "CWA":
            raise GradeEstimatorError("Must call calculate_cwa() before recalculate_cwa()")
        
        if total_achievable_wa < 0:
            raise ValueError("Total achievable weighted average cannot be negative")
        self._validate_credits(total_achievable_credits, "Total achievable credits")
        
        try:
            result = self.lib.recalculate_dist_CWA(
                self._student_obj,
                ctypes.c_float(total_achievable_wa),
                ctypes.c_int(total_achievable_credits)
            )
            return float(result)
        except Exception as e:
            self._destroy_student_object()
            raise GradeEstimatorError(f"CWA recalculation failed: {e}")
    
    def calculate_cgpa(self, current_cgpa: float, target_cgpa: float,
                      credits_completed: int, credits_remaining: int) -> float:
        """
        Calculate the required CGPA distribution.
        
        Args:
            current_cgpa: Student's current cumulative GPA (0-4.0)
            target_cgpa: Desired target cumulative GPA (0-4.0)
            credits_completed: Number of credits already completed
            credits_remaining: Number of credits left to complete
            
        Returns:
            Required CGPA per remaining course
            
        Raises:
            ValueError: If inputs are invalid
            GradeEstimatorError: If calculation fails
        """
        self._validate_cgpa_inputs(current_cgpa, target_cgpa)
        self._validate_credits(credits_completed, "Credits completed")
        self._validate_credits(credits_remaining, "Credits remaining")
        
        # Check for mode conflict
        if self._current_mode == "CWA":
            raise GradeEstimatorError("Cannot mix CWA and CGPA calculations. Create a new instance.")
        
        # Initialize fresh student object for new calculation
        self._init_student_object(credits_completed, credits_remaining)
        self._current_mode = "CGPA"
        
        try:
            result = self.lib.calculate_dist_CGPA(
                self._student_obj,
                ctypes.c_float(target_cgpa),
                ctypes.c_float(current_cgpa)
            )
            return float(result)
        except Exception as e:
            self._destroy_student_object()
            raise GradeEstimatorError(f"CGPA calculation failed: {e}")
    
    def recalculate_cgpa(self, total_achievable_grade_pts: float, total_achievable_credits: int) -> float:
        """
        Recalculate CGPA requirements after locking additional courses.
        
        Must be called after calculate_cgpa() on the same instance.
        
        Args:
            total_achievable_grade_pts: Total achievable grade points for locked courses
            total_achievable_credits: Sum of credit hours for locked courses
            
        Returns:
            Updated required CGPA per remaining course
            
        Raises:
            ValueError: If inputs are invalid
            GradeEstimatorError: If calculation fails or called before calculate_cgpa()
        """
        if self._student_obj is None or self._current_mode != "CGPA":
            raise GradeEstimatorError("Must call calculate_cgpa() before recalculate_cgpa()")
        
        if total_achievable_grade_pts < 0:
            raise ValueError("Total achievable grade points cannot be negative")
        self._validate_credits(total_achievable_credits, "Total achievable credits")
        
        try:
            result = self.lib.recalculate_dist_CGPA(
                self._student_obj,
                ctypes.c_float(total_achievable_grade_pts),
                ctypes.c_int(total_achievable_credits)
            )
            return float(result)
        except Exception as e:
            self._destroy_student_object()
            raise GradeEstimatorError(f"CGPA recalculation failed: {e}")
    
    def reset(self):
        """
        Reset the calculator by destroying the current student object.
        Call this when you want to start a completely new calculation.
        """
        self._destroy_student_object()
    
    def __enter__(self):
        """Context manager entry"""
        return self
    
    def __exit__(self, exc_type, exc_val, exc_tb):
        """Context manager exit - ensures cleanup"""
        self._destroy_student_object()
    
    def __del__(self):
        """Destructor - cleanup when object is garbage collected"""
        self._destroy_student_object()


# Example usage
if __name__ == "__main__":
    # Example 1: CWA Calculation
    print("=== CWA Example ===")
    with GradeEstimator("./libgrade_estimator.so") as estimator:
        # Calculate required CWA
        # Student has completed 60 credits with 75.5 CWA
        # Wants to achieve 85.0 CWA with 30 credits remaining
        required_cwa = estimator.calculate_cwa(
            current_cwa=75.5,
            target_cwa=85.0,
            credits_completed=60,
            credits_remaining=30
        )
        print(f"Required CWA per remaining credit: {required_cwa:.2f}")
        
        # Lock some courses (e.g., 3 credits with 90.0 WA = 270.0 total WA)
        updated_cwa = estimator.recalculate_cwa(
            total_achievable_wa=270.0,  # 3 credits * 90.0 WA
            total_achievable_credits=3
        )
        print(f"Updated required CWA after locking courses: {updated_cwa:.2f}")
    
    print("\n=== CGPA Example ===")
    # Example 2: CGPA Calculation (separate instance)
    with GradeEstimator("./libgrade_estimator.so") as estimator:
        # Calculate required CGPA
        # Student has completed 60 credits with 3.0 CGPA
        # Wants to achieve 3.5 CGPA with 30 credits remaining
        required_cgpa = estimator.calculate_cgpa(
            current_cgpa=3.0,
            target_cgpa=3.5,
            credits_completed=60,
            credits_remaining=30
        )
        print(f"Required CGPA per remaining credit: {required_cgpa:.2f}")
        
        # Lock some courses (e.g., 3 credits with 4.0 GPA = 12.0 grade points)
        updated_cgpa = estimator.recalculate_cgpa(
            total_achievable_grade_pts=12.0,  # 3 credits * 4.0 GPA
            total_achievable_credits=3
        )
        print(f"Updated required CGPA after locking courses: {updated_cgpa:.2f}")