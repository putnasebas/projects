package cz.cvut.fit.tjv.realestates.application;

import cz.cvut.fit.tjv.realestates.domain.Owner;
import cz.cvut.fit.tjv.realestates.repository.JPAOwnerRepository;
import jakarta.persistence.EntityNotFoundException;
import jakarta.transaction.Transactional;
import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.boot.test.context.SpringBootTest;

import java.util.List;

import static org.junit.jupiter.api.Assertions.*;

@SpringBootTest
@Transactional
public class OwnerServiceIntegrationTest {
    @Autowired
    private JPAOwnerRepository ownerRepository;
    private OwnerService ownerService;

    @BeforeEach
    public void setUp() {
        ownerService = new OwnerServiceImpl(ownerRepository);
    }

    @Test
    public void testSaveOwner() {
        Owner expectedOwner = new Owner("RandomMail@mail.com", null, List.of());
        Owner savedOwner = ownerService.createEntity(expectedOwner);
        assertNotNull(savedOwner);
        Owner actualOwner = ownerService.getEntityById(expectedOwner.getEmail());
        assertNotNull(actualOwner);
        assertEquals(expectedOwner.getEmail(), actualOwner.getEmail());
    }

    @Test
    public void testUpdateOwner() {
        Owner oldOwner = new Owner("Owner'sMail@mail.com", null, List.of());
        ownerService.createEntity(oldOwner);
        Owner expectedNewOwner = new Owner("Owner'sMail@mail.com", "785659455", List.of());
        Owner updatedOwner = ownerService.updateEntity(expectedNewOwner);
        assertNotNull(updatedOwner);
        assertEquals(expectedNewOwner.getEmail(), updatedOwner.getEmail());
        assertEquals(expectedNewOwner.getPhoneNumber(), updatedOwner.getPhoneNumber());
        // Check Owner accessibility from DB
        Owner actualOwner = ownerService.getEntityById(updatedOwner.getEmail());
        assertNotNull(actualOwner);
        assertEquals(expectedNewOwner.getEmail(), actualOwner.getEmail());
        assertEquals(expectedNewOwner.getPhoneNumber(), actualOwner.getPhoneNumber());
    }

    @Test
    public void testDeleteOwner() {
        Owner ownerToDelete = new Owner("Delete@mail.com", null, List.of());
        ownerService.createEntity(ownerToDelete);
        Owner savedOwner = ownerService.getEntityById(ownerToDelete.getEmail());
        assertNotNull(savedOwner);
        assertEquals(ownerToDelete.getEmail(), savedOwner.getEmail());

        ownerService.deleteEntity(ownerToDelete.getEmail());
        //Check if owner is no longer present within database
        assertThrowsExactly(EntityNotFoundException.class, () -> ownerService.getEntityById(ownerToDelete.getEmail()));
    }
}
