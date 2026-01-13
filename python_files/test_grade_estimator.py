from grade_estimator_wrapper import GradeEstimator, GradeEstimatorError

def test_cwa():
    """Test CWA calculations"""
    print("=" * 50)
    print("Testing CWA Calculation")
    print("=" * 50)
    
    try:
        # Update this path to match your library location
        # Linux/macOS: "./libgrade_estimator.so" or "./libgrade_estimator.dylib"
        # Windows: "./grade_estimator.dll" or "grade_estimator.dll"
        estimator = GradeEstimator("./libgrade_estimator.so")
        
        # Test basic calculation
        print("\nTest 1: Basic CWA Calculation")
        print("Current CWA: 75.5")
        print("Target CWA: 85.0")
        print("Credits Completed: 60")
        print("Credits Remaining: 30")
        
        required = estimator.calculate_cwa(
            current_cwa=75.5,
            target_cwa=85.0,
            credits_completed=60,
            credits_remaining=30
        )
        print(f"Result: Required CWA = {required:.2f}")
        
        # Test recalculation
        print("\nTest 2: Recalculation after locking courses")
        print("Locking 3 credits with 90.0 weighted average")
        
        updated = estimator.recalculate_cwa(
            total_achievable_wa=270.0,  # 3 * 90.0
            total_achievable_credits=3
        )
        print(f"Result: Updated required CWA = {updated:.2f}")
        
        # Clean up
        estimator.reset()
        print("\n✓ CWA tests passed!")
        
    except GradeEstimatorError as e:
        print(f"\n✗ Grade Estimator Error: {e}")
    except ValueError as e:
        print(f"\n✗ Validation Error: {e}")
    except Exception as e:
        print(f"\n✗ Unexpected Error: {e}")


def test_cgpa():
    """Test CGPA calculations"""
    print("\n" + "=" * 50)
    print("Testing CGPA Calculation")
    print("=" * 50)
    
    try:
        estimator = GradeEstimator("./libgrade_estimator.so")
        
        # Test basic calculation
        print("\nTest 1: Basic CGPA Calculation")
        print("Current CGPA: 3.0")
        print("Target CGPA: 3.5")
        print("Credits Completed: 60")
        print("Credits Remaining: 30")
        
        required = estimator.calculate_cgpa(
            current_cgpa=3.0,
            target_cgpa=3.5,
            credits_completed=60,
            credits_remaining=30
        )
        print(f"Result: Required CGPA = {required:.2f}")
        
        # Test recalculation
        print("\nTest 2: Recalculation after locking courses")
        print("Locking 3 credits with 4.0 GPA (12.0 grade points)")
        
        updated = estimator.recalculate_cgpa(
            total_achievable_grade_pts=12.0,  # 3 * 4.0
            total_achievable_credits=3
        )
        print(f"Result: Updated required CGPA = {updated:.2f}")
        
        # Clean up
        estimator.reset()
        print("\n✓ CGPA tests passed!")
        
    except GradeEstimatorError as e:
        print(f"\n✗ Grade Estimator Error: {e}")
    except ValueError as e:
        print(f"\n✗ Validation Error: {e}")
    except Exception as e:
        print(f"\n✗ Unexpected Error: {e}")


def test_context_manager():
    """Test using the context manager (with statement)"""
    print("\n" + "=" * 50)
    print("Testing Context Manager")
    print("=" * 50)
    
    try:
        with GradeEstimator("./libgrade_estimator.so") as estimator:
            result = estimator.calculate_cwa(
                current_cwa=70.0,
                target_cwa=80.0,
                credits_completed=30,
                credits_remaining=30
            )
            print(f"Required CWA using 'with' statement: {result:.2f}")
        
        print("✓ Context manager automatically cleaned up!")
        
    except Exception as e:
        print(f"✗ Error: {e}")


def test_error_handling():
    """Test error handling"""
    print("\n" + "=" * 50)
    print("Testing Error Handling")
    print("=" * 50)
    
    try:
        estimator = GradeEstimator("./libgrade_estimator.so")
        
        # Test invalid CWA (should fail)
        print("\nTest 1: Invalid CWA (150 - out of range)")
        try:
            estimator.calculate_cwa(
                current_cwa=150.0,  # Invalid!
                target_cwa=85.0,
                credits_completed=60,
                credits_remaining=30
            )
            print("✗ Should have raised ValueError!")
        except ValueError as e:
            print(f"✓ Correctly caught error: {e}")
        
        # Test mixing CWA and CGPA (should fail)
        print("\nTest 2: Mixing CWA and CGPA")
        try:
            estimator.calculate_cwa(75.0, 85.0, 60, 30)
            estimator.calculate_cgpa(3.0, 3.5, 60, 30)  # Should fail
            print("✗ Should have raised GradeEstimatorError!")
        except GradeEstimatorError as e:
            print(f"✓ Correctly caught error: {e}")
        
        estimator.reset()
        
    except Exception as e:
        print(f"✗ Unexpected error: {e}")


if __name__ == "__main__":
    print("\n")
    print("╔" + "=" * 48 + "╗")
    print("║  GRADE ESTIMATOR PYTHON WRAPPER - TEST SUITE  ║")
    print("╚" + "=" * 48 + "╝")
    
    test_cwa()
    test_cgpa()
    test_context_manager()
    test_error_handling()
    
    print("\n" + "=" * 50)
    print("All tests completed!")
    print("=" * 50 + "\n")