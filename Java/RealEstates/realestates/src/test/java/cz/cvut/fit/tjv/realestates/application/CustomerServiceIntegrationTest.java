package cz.cvut.fit.tjv.realestates.application;

import cz.cvut.fit.tjv.realestates.domain.Customer;
import cz.cvut.fit.tjv.realestates.repository.JPACustomerRepository;
import jakarta.persistence.EntityNotFoundException;
import jakarta.transaction.Transactional;
import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.boot.test.context.SpringBootTest;

import java.time.LocalDate;
import java.util.List;

import static org.junit.jupiter.api.Assertions.*;

@SpringBootTest
@Transactional
public class CustomerServiceIntegrationTest {
    @Autowired
    private JPACustomerRepository customerRepository;
    private CustomerService customerService;

    @BeforeEach
    public void setUp() {
        customerService = new CustomerServiceImpl(customerRepository);
    }

    @Test
    public void testSaveCustomer() {
        Customer expectedCustomer = new Customer("Java.Java@mail.com", "Java", "Java",
                LocalDate.of(1988, 12, 11), List.of());
        Customer savedCustomer = customerService.createEntity(expectedCustomer);
        assertNotNull(savedCustomer);
        // Test if expectedCustomer was saved properly
        Customer customerInRepository = customerService.getEntityById(expectedCustomer.getEmail());
        assertNotNull(customerInRepository);
        assertEquals(expectedCustomer.getEmail(), customerInRepository.getEmail());
        assertEquals(expectedCustomer.getFirstName(), customerInRepository.getFirstName());
        assertEquals(expectedCustomer.getLastName(), customerInRepository.getLastName());
        assertEquals(expectedCustomer.getBirthDate(), customerInRepository.getBirthDate());
        assertEquals(expectedCustomer.getSubleases(), customerInRepository.getSubleases());
    }

    @Test
    public void testUpdateCustomer() {
        Customer oldCustomer = new Customer("Java.Java@mail.com", "Java", "Java",
                LocalDate.of(1988, 12, 11), List.of());
        customerService.createEntity(oldCustomer);
        Customer expectedNewCustomer = new Customer("Java.Java@mail.com", "Java", "Java",
                LocalDate.of(1980, 12, 11), List.of());

        Customer updatedCustomer = customerService.updateEntity(expectedNewCustomer);
        assertNotNull(updatedCustomer);
        assertEquals(expectedNewCustomer.getEmail(), updatedCustomer.getEmail());
        assertEquals(expectedNewCustomer.getFirstName(), updatedCustomer.getFirstName());
        assertEquals(expectedNewCustomer.getLastName(), updatedCustomer.getLastName());
        assertEquals(expectedNewCustomer.getBirthDate(), updatedCustomer.getBirthDate());
        assertEquals(expectedNewCustomer.getSubleases(), updatedCustomer.getSubleases());
        Customer actualCustomer = customerService.getEntityById(updatedCustomer.getEmail());
        assertNotNull(actualCustomer);
        assertEquals(expectedNewCustomer.getEmail(), actualCustomer.getEmail());
        assertEquals(expectedNewCustomer.getFirstName(), actualCustomer.getFirstName());
        assertEquals(expectedNewCustomer.getLastName(), actualCustomer.getLastName());
        assertEquals(expectedNewCustomer.getBirthDate(), actualCustomer.getBirthDate());
        assertEquals(expectedNewCustomer.getSubleases(), actualCustomer.getSubleases());
    }

    @Test
    public void testDeleteCustomer() {
        Customer customerToDelete = new Customer("Java.Java@mail.com", "Java", "Java",
                LocalDate.of(1988, 12, 11), List.of());
        customerService.createEntity(customerToDelete);
        customerService.deleteEntity(customerToDelete.getEmail());
        // Check if customer is no longer present within the database
        assertThrowsExactly(EntityNotFoundException.class,()-> {
            customerService.getEntityById(customerToDelete.getEmail());
        });
    }
}
